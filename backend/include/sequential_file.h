#ifndef SEQUENTIAL_FILE_H
#define SEQUENTIAL_FILE_H

#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <cstring>

template<typename Key, typename T>
class SequentialFile {
private:
    std::string dataFileName;
    std::string auxFileName;
    int MAX_REBUILD_FACTOR = 50;

public:
    SequentialFile(const std::string& dataFileName, const std::string& auxFileName);

    void insertIntoAuxFile(T & record, int& reads, int& writes);
    void updatePointers(T & record, int newRecordPosition, int& reads, int& writes);
    void updateDataFile(T & record, int& reads, int& writes);
    void updateAuxFile(T & previousRecord, T& record, int& reads, int& writes);
    void rebuildSequential(int& reads, int& writes);
    void checkRebuild(int& reads, int& writes);
    T search_by_key(Key keyint, int& reads, int& writes);
    std::vector<T> get_all(int& reads, int& writes);
    std::vector<T> get_between(Key key_begin, Key key_end, int& reads, int& writes);
    bool delete_by_key(Key key, int& reads, int& writes);
};


template<typename Key, typename T>
SequentialFile<Key, T>::SequentialFile(const std::string& dataFileName, const std::string& auxFileName)
    : dataFileName(dataFileName), auxFileName(auxFileName) {
    std::ifstream dataFile(dataFileName, std::ios::binary);
    if (!dataFile.is_open()) {
        // Crear el archivo datafile con un registro inicial (dummy)
        std::ofstream outFile(dataFileName, std::ios::binary);
        T dummy;
        dummy.key = -1;
        dummy.pos = 0; // Dummy es la primera posición en el datafile
        outFile.write(reinterpret_cast<const char*>(&dummy), sizeof(T));
        outFile.close();
    }
}

// Insertar en el archivo auxiliar
template<typename Key, typename T>
void SequentialFile<Key, T>::insertIntoAuxFile(T& record, int& reads, int& writes) {
    std::ofstream auxFile(auxFileName, std::ios::binary | std::ios::app);
    auxFile.seekp(0, std::ios::end);
    int newRecordPosition = auxFile.tellp() / sizeof(T); // Calcular posición
    record.pos = newRecordPosition; // Asignar la posición al registro
    auxFile.write(reinterpret_cast<const char*>(&record), sizeof(T));
    auxFile.close();
    writes++;

    updatePointers(record, newRecordPosition, reads, writes); 
    this->checkRebuild(reads, writes);
}

template<typename Key, typename T>
void SequentialFile<Key, T>::updatePointers(T& record, int newRecordPosition, int& reads, int& writes) {
    T currentRecord;

    std::ifstream dataFile(dataFileName, std::ios::binary);
    reads++;
    if (dataFile.read(reinterpret_cast<char*>(&currentRecord), sizeof(T))) {
        // Si el dummy no apunta a ningún registro, actualizamos directamente
        if (currentRecord.nextRecord == -1) {
            currentRecord.nextRecord = record.pos; // Actualizar para que apunte al nuevo registro
            currentRecord.nextFileChar = 'a'; // Apunta al archivo auxiliar
            currentRecord.accFile = 'd'; // Indica que está en datafile

            updateDataFile(currentRecord, reads, writes); // Guardar el dummy actualizado en datafile
            return; // No hay más que actualizar en esta primera inserción
        }

        while (true) {
            if (currentRecord.nextRecord == -1) {
                if (currentRecord.accFile == 'd') {
                    currentRecord.nextRecord = record.pos; // Actualizar para que apunte al nuevo registro
                    currentRecord.nextFileChar = 'a'; // Ahora apunta al archivo auxiliar
                    updateDataFile(currentRecord, reads, writes); // Guardar en datafile
                } else {
                    currentRecord.nextRecord = record.pos; // Actualizar para que apunte al nuevo registro
                    currentRecord.nextFileChar = 'a'; // Ahora apunta al datafile
                    updateAuxFile(currentRecord, currentRecord, reads, writes); // Guardar en auxfile
                }

                // Actualizar el nuevo registro para que apunte a -1 (final)
                record.nextRecord = -1;
                record.nextFileChar = 'a'; // El nuevo registro se guarda en auxfile
                updateAuxFile(currentRecord, record, reads, writes); // Guardar el nuevo registro en auxfile
                return;
            }

            T nextRecord;
            std::ifstream nextFile;

            // Abrir el archivo correspondiente según nextFileChar
            if (currentRecord.nextFileChar == 'd') {
                nextFile.open(dataFileName, std::ios::binary);
            } else {
                nextFile.open(auxFileName, std::ios::binary);
            }

            // Ir a la posición del siguiente registro
            nextFile.seekg(currentRecord.nextRecord * sizeof(T), std::ios::beg);
            reads++;
            if (!nextFile.read(reinterpret_cast<char*>(&nextRecord), sizeof(T))) {
                break; // No hay más registros para comparar
            }

            if (nextRecord.key > record.key) {
                // Actualizar el currentRecord para que apunte al nuevo registro
                currentRecord.nextRecord = record.pos; 
                currentRecord.nextFileChar = 'a'; // Ahora apunta al archivo auxiliar

                // Guardar el currentRecord en su respectivo archivo
                if (currentRecord.accFile == 'd') {
                    updateDataFile(currentRecord, reads, writes); // Guardar en datafile
                } else {
                    updateAuxFile(currentRecord, currentRecord, reads, writes); // Guardar en auxfile
                }

                // Actualizar el nuevo registro para que apunte al nextRecord
                record.nextRecord = nextRecord.pos;
                record.nextFileChar = nextRecord.accFile;

                // Guardar el nuevo registro en el archivo auxiliar
                updateAuxFile(currentRecord, record, reads, writes);
                return;
            }

            // Si no hay que actualizar, seguimos al siguiente registro
            currentRecord = nextRecord;
        }
    }
    dataFile.close();
}


// Actualiza el archivo datafile con el registro dado
template<typename Key, typename T>
void SequentialFile<Key, T>::updateDataFile(T& record, int& reads, int& writes) {
    std::fstream dataFile(dataFileName, std::ios::binary | std::ios::in | std::ios::out);
    dataFile.seekp(record.pos * sizeof(T), std::ios::beg);
    dataFile.write(reinterpret_cast<const char*>(&record), sizeof(T));
    writes++;
    dataFile.close();
}

// Actualiza el archivo auxfile con el registro dado
template<typename Key, typename T>
void SequentialFile<Key, T>::updateAuxFile(T& previousRecord, T& record, int& reads, int& writes) {
    std::fstream auxFile(auxFileName, std::ios::binary | std::ios::in | std::ios::out);
    auxFile.seekp(record.pos * sizeof(T), std::ios::beg);
    auxFile.write(reinterpret_cast<const char*>(&record), sizeof(T));
    writes++;
    auxFile.close();
}

template<typename Key, typename T>
void SequentialFile<Key, T>::rebuildSequential(int& reads, int& writes) {
    std::ifstream dataFile(dataFileName, std::ios::binary);
    std::ifstream auxFile(auxFileName, std::ios::binary);
    std::ofstream tmpDataFile("tmp_datafile.dat", std::ios::binary);
    
    if (!dataFile.is_open() || !auxFile.is_open() || !tmpDataFile.is_open()) {
        std::cout << "Error opening files." << std::endl;
        return;
    }

    T currentRecord;
    int counter = 0;

    // Read the first record from dataFile
    reads++;
    if (!dataFile.read(reinterpret_cast<char*>(&currentRecord), sizeof(T))) {
        // If the data file is empty, just return
        dataFile.close();
        auxFile.close();
        tmpDataFile.close();
        return;
    }

    // Skip the dummy record if it has a key of -1
    if (currentRecord.key == -1) {
        reads++;
        if(currentRecord.nextFileChar == 'd'){
            dataFile.seekg(currentRecord.nextRecord * sizeof(T), std::ios::beg);
            dataFile.read(reinterpret_cast<char*>(&currentRecord), sizeof(T));
        } else {
            auxFile.seekg(currentRecord.nextRecord * sizeof(T), std::ios::beg);
            auxFile.read(reinterpret_cast<char*>(&currentRecord), sizeof(T));
        }
    }

    // Process records until nextRecord is -1
    while (true) {
        char nextFileChar = currentRecord.nextFileChar;
        int nextRecordPosition = currentRecord.nextRecord;

        // Update the current record's position and file attributes
        currentRecord.pos = counter; // Update pos to current count
        currentRecord.nextFileChar = 'd'; // Set to 'd' indicating datafile
        if(currentRecord.nextRecord == -1){
          currentRecord.nextRecord = -1; // Point to the next position
        }else {
          currentRecord.nextRecord = counter + 1; // Point to the next position
        }
        currentRecord.accFile = 'd'; // Indicate that it's in datafile

        // Write the updated current record to the temporary data file
        writes++;
        tmpDataFile.write(reinterpret_cast<char*>(&currentRecord), sizeof(T));
        if(currentRecord.nextRecord == -1) break;
        
        // Increment the counter
        counter++;

        // Read the next record based on nextFileChar
        reads++;
        if (nextFileChar == 'd') {
            // Read from dataFile
            dataFile.seekg(nextRecordPosition * sizeof(T), std::ios::beg);
            if (!dataFile.read(reinterpret_cast<char*>(&currentRecord), sizeof(T))) {
                break; // No more records in dataFile
            }
        } else {
            // Read from auxFile
            auxFile.seekg(nextRecordPosition * sizeof(T), std::ios::beg);
            if (!auxFile.read(reinterpret_cast<char*>(&currentRecord), sizeof(T))) {
                break; // No more records in auxFile
            }
        }
    }

    // Close all files
    dataFile.close();
    auxFile.close();
    tmpDataFile.close();
    std::ofstream clearAuxFile(auxFileName, std::ios::binary | std::ios::trunc);
    clearAuxFile.close();

    // Replace the original data file with the new temporary file
    std::remove(dataFileName.c_str());
    std::rename("tmp_datafile.dat", dataFileName.c_str());
}

template<typename Key, typename T>
T SequentialFile<Key, T>::search_by_key(Key key, int& reads, int& writes){
    T result;
    std::ifstream dataFile(dataFileName, std::ios::binary);
    std::ifstream auxFile(auxFileName, std::ios::binary);
    if (!dataFile.is_open() || !auxFile.is_open()) {
        std::cout << "Error opening files." << std::endl;
        return result;
    }

    T currentRecord;

    reads++;
    if (!dataFile.read(reinterpret_cast<char*>(&currentRecord), sizeof(T))) {
        dataFile.close();
        auxFile.close();
        return result;
    }

    reads++;
    if (currentRecord.key == -1) {
        if(currentRecord.nextFileChar == 'd'){
            dataFile.seekg(currentRecord.nextRecord * sizeof(T), std::ios::beg);
            dataFile.read(reinterpret_cast<char*>(&currentRecord), sizeof(T));
        } else {
            auxFile.seekg(currentRecord.nextRecord * sizeof(T), std::ios::beg);
            auxFile.read(reinterpret_cast<char*>(&currentRecord), sizeof(T));
        }
    }

    while (true) {
        char nextFileChar = currentRecord.nextFileChar;
        int nextRecordPosition = currentRecord.nextRecord;
        if(currentRecord.key == key){
          result = currentRecord;
          break;
        } else if (currentRecord.key > key) break;
        reads++;
        if (nextFileChar == 'd') {
            // Read from dataFile
            dataFile.seekg(nextRecordPosition * sizeof(T), std::ios::beg);
            if (!dataFile.read(reinterpret_cast<char*>(&currentRecord), sizeof(T))) {
                break; // No more records in dataFile
            }
        } else {
            // Read from auxFile
            auxFile.seekg(nextRecordPosition * sizeof(T), std::ios::beg);
            if (!auxFile.read(reinterpret_cast<char*>(&currentRecord), sizeof(T))) {
                break; // No more records in auxFile
            }
        }
    }
    dataFile.close();
    auxFile.close();
    return result;

}

template<typename Key, typename T>
std::vector<T> SequentialFile<Key, T>::get_all(int& reads, int& writes) {
    std::vector<T> all_records;
    std::ifstream dataFile(dataFileName, std::ios::binary);
    std::ifstream auxFile(auxFileName, std::ios::binary);
    if (!dataFile.is_open() || !auxFile.is_open()) {
        std::cerr << "Error opening files." << std::endl;
        return all_records;
    }

    T currentRecord;

    reads++;
    if (!dataFile.read(reinterpret_cast<char*>(&currentRecord), sizeof(T))) {
        dataFile.close();
        auxFile.close();
        return all_records;
    }

    reads++;
    if (currentRecord.key == -1) {
        if(currentRecord.nextFileChar == 'd'){
            dataFile.seekg(currentRecord.nextRecord * sizeof(T), std::ios::beg);
            dataFile.read(reinterpret_cast<char*>(&currentRecord), sizeof(T));
        } else {
            auxFile.seekg(currentRecord.nextRecord * sizeof(T), std::ios::beg);
            auxFile.read(reinterpret_cast<char*>(&currentRecord), sizeof(T));
        }
    }

    while (true) {
        char nextFileChar = currentRecord.nextFileChar;
        int nextRecordPosition = currentRecord.nextRecord;
        all_records.push_back(currentRecord);
        reads++;
        if (nextFileChar == 'd') {
            // Read from dataFile
            dataFile.seekg(nextRecordPosition * sizeof(T), std::ios::beg);
            if (!dataFile.read(reinterpret_cast<char*>(&currentRecord), sizeof(T))) {
                break; // No more records in dataFile
            }
        } else {
            // Read from auxFile
            auxFile.seekg(nextRecordPosition * sizeof(T), std::ios::beg);
            if (!auxFile.read(reinterpret_cast<char*>(&currentRecord), sizeof(T))) {
                break; // No more records in auxFile
            }
        }
    }
    dataFile.close();
    auxFile.close();
    return all_records;
}

template<typename Key, typename T>
void SequentialFile<Key, T>::checkRebuild(int& reads, int& writes) {
    std::ifstream auxFile(auxFileName, std::ios::binary);
    if (!auxFile.is_open()) {
        std::cerr << "Error opening files." << std::endl;
        return;
    }
    auxFile.seekg(0, std::ios::end);
    std::streamsize fileSize = auxFile.tellg();
    auxFile.seekg(0, std::ios::beg);  
    size_t recordSize = sizeof(T);
    size_t numRecords = fileSize / recordSize;
    if(numRecords == MAX_REBUILD_FACTOR){
      std::cout << "REBUILD FACTOR \n";
      this->rebuildSequential(reads, writes);
    }
    auxFile.close();
}

template<typename Key, typename T>
std::vector<T> SequentialFile<Key, T>::get_between(Key key_begin, Key key_end, int& reads, int& writes) {
    std::vector<T> range_records;
    std::ifstream dataFile(dataFileName, std::ios::binary);
    std::ifstream auxFile(auxFileName, std::ios::binary);
    if (!dataFile.is_open() || !auxFile.is_open()) {
        std::cerr << "Error opening files." << std::endl;
        return range_records;
    }

    T currentRecord;

    reads++;
    if (!dataFile.read(reinterpret_cast<char*>(&currentRecord), sizeof(T))) {
        dataFile.close();
        auxFile.close();
        return range_records;
    }

    reads++;
    if (currentRecord.key == -1) {
        if(currentRecord.nextFileChar == 'd'){
            dataFile.seekg(currentRecord.nextRecord * sizeof(T), std::ios::beg);
            dataFile.read(reinterpret_cast<char*>(&currentRecord), sizeof(T));
        } else {
            auxFile.seekg(currentRecord.nextRecord * sizeof(T), std::ios::beg);
            auxFile.read(reinterpret_cast<char*>(&currentRecord), sizeof(T));
        }
    }

    while (true) {
        char nextFileChar = currentRecord.nextFileChar;
        int nextRecordPosition = currentRecord.nextRecord;
        if(key_begin <= currentRecord.key && currentRecord.key  <= key_end){
          range_records.push_back(currentRecord);
        } else if (currentRecord.key > key_end) break;
        reads++;
        if (nextFileChar == 'd') {
            // Read from dataFile
            dataFile.seekg(nextRecordPosition * sizeof(T), std::ios::beg);
            if (!dataFile.read(reinterpret_cast<char*>(&currentRecord), sizeof(T))) {
                break; // No more records in dataFile
            }
        } else {
            // Read from auxFile
            auxFile.seekg(nextRecordPosition * sizeof(T), std::ios::beg);
            if (!auxFile.read(reinterpret_cast<char*>(&currentRecord), sizeof(T))) {
                break; // No more records in auxFile
            }
        }
    }
    dataFile.close();
    auxFile.close();
    return range_records;
}


template<typename Key, typename T>
bool SequentialFile<Key, T>::delete_by_key(Key key, int& reads, int& writes){
    std::fstream dataFile(dataFileName, std::ios::in | std::ios::out | std::ios::binary);
    std::fstream auxFile(auxFileName, std::ios::in | std::ios::out | std::ios::binary);

    if (!dataFile.is_open() || !auxFile.is_open()) {
        std::cerr << "Error opening files." << std::endl;
        return false;
    }

    T currentRecord;

    reads++;
    if (!dataFile.read(reinterpret_cast<char*>(&currentRecord), sizeof(T))) {
        dataFile.close();
        auxFile.close();
        return false;
    }

    if (currentRecord.key == -1) {
        reads++;
        if(currentRecord.nextFileChar == 'd'){
            dataFile.seekg(currentRecord.nextRecord * sizeof(T), std::ios::beg);
            dataFile.read(reinterpret_cast<char*>(&currentRecord), sizeof(T));
        } else {
            auxFile.seekg(currentRecord.nextRecord * sizeof(T), std::ios::beg);
            auxFile.read(reinterpret_cast<char*>(&currentRecord), sizeof(T));
        }
    }

    while (true) {
        char nextFileChar = currentRecord.nextFileChar;
        int nextRecordPosition = currentRecord.nextRecord;
        T tmp_record;
        reads++;
        if (nextFileChar == 'd') {
            // Read from dataFile
            dataFile.seekg(nextRecordPosition * sizeof(T), std::ios::beg);
            if (!dataFile.read(reinterpret_cast<char*>(&tmp_record), sizeof(T))) {
                break; // No more records in dataFile
            }
        } else {
            // Read from auxFile
            auxFile.seekg(nextRecordPosition * sizeof(T), std::ios::beg);
            if (!auxFile.read(reinterpret_cast<char*>(&tmp_record), sizeof(T))) {
                break; // No more records in auxFile
            }
        }
        if(tmp_record.key == key){
          currentRecord.nextRecord = tmp_record.nextRecord;
          currentRecord.nextFileChar = tmp_record.nextFileChar;
          reads++;
          if(currentRecord.accFile == 'a'){
            auxFile.seekp(currentRecord.pos * sizeof(T), std::ios::beg);
            auxFile.write(reinterpret_cast<const char*>(&currentRecord), sizeof(T));
          } else {
            dataFile.seekp(currentRecord.pos * sizeof(T), std::ios::beg);
            dataFile.write(reinterpret_cast<const char*>(&currentRecord), sizeof(T));
          }
          return true;
        } else if (currentRecord.key > key) break;
        currentRecord = tmp_record;
    }
    dataFile.close();
    auxFile.close();
    return false;
}



#endif

