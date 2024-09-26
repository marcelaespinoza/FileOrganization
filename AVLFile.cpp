#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstring>
#include <deque>
using namespace std;

struct Record //48
{
    int cod; //4
    char nombre[12]; //12
    int ciclo; //4

    long left, right;

    int height; //Empieza en 0

    Record(){}
    Record(int cod, const char* nombre, int ciclo)
            : cod(cod), ciclo(ciclo), left(-1), right(-1), height(0) {
        strncpy(this->nombre, nombre, sizeof(this->nombre) - 1);
        this->nombre[sizeof(this->nombre) - 1] = '\0';
    }
    void setData() {
        cout << "Codigo:";
        cin >> cod;
        cout << "Nombre: ";
        cin >> nombre;
        cout << "Ciclo: ";
        cin >> ciclo;
        this->height = 0;
        this->left = -1;
        this->right = -1;
    }

    void showData() {
        cout << "\nCodigo: " << cod;
        cout << "\nNombre: " << nombre;
        cout << "\nCiclo: " << ciclo;
        cout << "\nLeft: " << left;
        cout << "\nRight: " << right;
        cout << "\nHeight : " << height <<endl;
    }
};

class AVLFile{
private:
    string filename;
    long pos_root;
public:
    AVLFile(string filename){
        this->filename = filename;

        fstream file;
        file.open(this->filename, ios::in | ios::binary);

        if(!file.is_open()) {
            this->pos_root = -1;
        }else{
            this->pos_root = 0;
        }
        file.close();
    }

    void getFirst(){//Segundo
        fstream file;
        file.open(this->filename, ios::in | ios::binary);

        Record record;
        while (file.read((char*)&record, sizeof(Record))) {
            record.showData();
            cout << "---------------------" << std::endl;
        }


        file.close();
    }

    Record find(int key){
        fstream file;
        file.open(this->filename, ios::in | ios::app| ios::binary);
        Record record = find(this->pos_root, key, file);
        file.close();
        return record;
    }

    void insert(Record record){
        //Se ingresa el registro al último, en caso no existan registros previos, se define como el root (archivo vacío)
        fstream file;
        file.open(this->filename, ios::in | ios::out | ios::binary);

        if(file.is_open()) {
            file.seekp(0, std::ios::end);
            file.write((char *) &record, sizeof(Record));
        }
        else {
            ofstream createFile(filename, std::ios::out | std::ios::binary);
            createFile.close();

            file.open(filename, std::ios::in | std::ios::out | std::ios::binary);
            file.write((char*) &record, sizeof(Record));
        }

        //En caso exista registro se debe obtener el root
        if(this->pos_root == -1){//cuando el archivo está vacío
            this->pos_root = 0; //apunta al primer registro
        }


        //ESTABLECER CONEXION (encontrar registro previo)
        file.seekp(0, ios::end);
        int pos_final = file.tellp()/sizeof(Record) - 1; //0

        //cout<<"Posicion final: "<<pos_final<<endl;

        deque<int> posiciones;
        conection(this->pos_root, record.cod, pos_final, file, posiciones);


        for(int i =0; i<posiciones.size(); i++){
            //Modificar altura
            updateHeight(posiciones[i], file);
            //BALANCEAR
            balance(file,posiciones[i]); //Corregir
        }

        file.close(); //terminada toda la inserción
    }

    vector<Record> inorder(){
        fstream file(this->filename, std::ios::in | std::ios::binary);

        vector<Record> result;
        if (this->pos_root != -1) {
            inorder(this->pos_root, result, file);
        }else cout<<"No hay elementos registrados"<<endl;

        file.close();

        return result;
    }


private:

    void balance(fstream &file, long pos_node) {
        if (pos_node == -1) return;
        Record node;
        file.seekg(pos_node * sizeof(Record));
        file.read((char*)&node, sizeof(Record));

        int balance = BalancingFactor(pos_node, file);
        if (balance > 1) {
            if (BalancingFactor(node.left, file) < 0) {
                leftRotate(file, node.left);
            }
            rightRotate(file, pos_node);
        }

        else if (balance < -1) {
            cout<<"HOLA2";
            if (BalancingFactor(node.right, file) > 0) {
                rightRotate(file, node.right);
            }
            leftRotate(file, pos_node);
        }

    }

    int BalancingFactor(long pos_node, fstream &file){
        Record record;
        file.seekg(pos_node *sizeof(Record), ios::beg);
        file.read((char *) &record, sizeof (Record));

        return height(record.left, file) - height(record.right, file);
    }

    void rightRotate(fstream &file, long y_pos) {
        Record y, x;
        file.seekg(y_pos * sizeof(Record));
        file.read((char*)&y, sizeof(Record));
        long x_pos = y.left;

        file.seekg(x_pos * sizeof(Record));
        file.read((char*)&x, sizeof(Record));

        y.left = x.right;
        x.right = y_pos;

        if (y_pos == 0) {
            swapRecords(file, 0, x_pos);
            updateLinks(file, x_pos, 0);
        } else {
            // Actualizar los registros en sus nuevas posiciones
            file.seekp(y_pos * sizeof(Record));
            file.write((char*)&y, sizeof(Record));

            file.seekp(x_pos * sizeof(Record));
            file.write((char*)&x, sizeof(Record));
        }

        updateHeight(y_pos, file);
        updateHeight(x_pos, file);
    }

    void leftRotate(fstream &file, long x_pos) {
        Record x, y;
        file.seekg(x_pos * sizeof(Record));
        file.read((char*)&x, sizeof(Record));
        long y_pos = x.right;

        file.seekg(y_pos * sizeof(Record));
        file.read((char*)&y, sizeof(Record));

        // Realizar rotación
        x.right = y.left;
        y.left = x_pos;

        if (x_pos == 0) {
            swapRecords(file, 0, y_pos);
            updateLinks(file, y_pos, 0);
        } else {
            file.seekp(x_pos * sizeof(Record));
            file.write((char*)&x, sizeof(Record));

            file.seekp(y_pos * sizeof(Record));
            file.write((char*)&y, sizeof(Record));
        }

        updateHeight(x_pos, file);
        updateHeight(y_pos, file);
    }

    void swapRecords(fstream &file, long pos1, long pos2) {
        if (pos1 == pos2) return;

        Record record1, record2;
        file.seekg(pos1 * sizeof(Record));
        file.read((char*)&record1, sizeof(Record));
        file.seekg(pos2 * sizeof(Record));
        file.read((char*)&record2, sizeof(Record));

        file.seekp(pos1 * sizeof(Record));
        file.write((char*)&record2, sizeof(Record));
        file.seekp(pos2 * sizeof(Record));
        file.write((char*)&record1, sizeof(Record));
    }

    void updateLinks(fstream &file, long oldPos, long newPos) {
        file.seekg(0, ios::end);
        long fileSize = file.tellg();
        long numRecords = fileSize / sizeof(Record);

        Record temp;
        for (long i = 0; i < numRecords; ++i) {
            file.seekg(i * sizeof(Record));
            file.read((char*)&temp, sizeof(Record));

            bool updated = false;
            if (temp.left == oldPos) {
                temp.left = newPos;
                updated = true;
            } else if (temp.left == newPos) {
                temp.left = oldPos;
                updated = true;
            }

            if (temp.right == oldPos) {
                temp.right = newPos;
                updated = true;
            } else if (temp.right == newPos) {
                temp.right = oldPos;
                updated = true;
            }

            if (updated) {
                file.seekp(i * sizeof(Record));
                file.write((char*)&temp, sizeof(Record));
            }
        }
    }



    Record find(long pos_node, int key, fstream &file){
        if(pos_node == -1){
            return Record();
        }
        Record record;
        file.seekg(pos_node *sizeof(Record), ios::beg);
        file.read((char *) &record, sizeof (Record));

        if(key <record.cod){
            return find(record.left, key, file);
        }else if(key >  record.cod){
            return find(record.right, key, file);

        }else{
            return record;
        }

    }

    void updateHeight(int pos_actual, fstream &file){
        //Accede a la posición del record en el archivo saca su data y la modifica
        Record record;
        file.seekg(pos_actual *sizeof(Record), ios::beg);
        file.read((char *) &record, sizeof (Record));

        record.height = max(height(record.left, file), height(record.right, file)) + 1;

        file.seekp(pos_actual * sizeof(Record), ios::beg);
        file.write((char *) &record, sizeof (Record));
        file.flush();
    }

    int height(long pos, fstream &file){
        if(pos == -1) return -1;
        else{
            Record record;
            file.seekg(pos *sizeof(Record), ios::beg);
            file.read((char *) &record, sizeof (Record));
            return record.height;
        }
    }

    void conection(long pos_node, int code, int pos_final, fstream &file, deque<int> &deq){
        if(pos_node != pos_final){

            Record record;
            file.seekg(pos_node *sizeof(Record), ios::beg);
            file.read((char *) &record, sizeof (Record));

            deq.push_front(pos_node);

            if(record.cod <  code){
                if(record.right != -1) conection(record.right, code, pos_final, file, deq);
                else{
                    record.right = pos_final;

                    file.seekp(pos_node*sizeof(Record), ios::beg);

                    if (!file.write((char *)&record, sizeof(Record))) {
                        cerr << "Fallo al escribir el registro." << endl;
                    }
                    file.flush();
                }
            }else{
                if(record.left != -1) conection(record.left, code, pos_final, file, deq);
                else{
                    record.left = pos_final;
                    file.seekp(pos_node*sizeof(Record), ios::beg);

                    if (!file.write((char *)&record, sizeof(Record))) {
                        cerr << "Fallo al escribir el registro." << endl;
                    }
                    file.flush();
                }

            }
        }

    }


    void inorder(long pos_node, vector<Record> &result, fstream& file){
        if (pos_node == -1) return;

        Record record;
        file.seekg(pos_node * sizeof(Record));
        file.read(reinterpret_cast<char*>(&record), sizeof(Record));

        inorder(record.left, result,file);
        result.push_back(record);
        inorder(record.right, result,file);
    }


};





void testAVLFile() {
    AVLFile file("data.dat");

    // Insertar datos de prueba
    vector<Record> testRecords = {
            {10, "Alice", 1}, {20, "Bob", 2}, {30, "Charlie", 3},
            {15, "Dave", 1}, {25, "Eve", 2}, {5, "Faythe", 3}
    };

    for (auto& rec : testRecords) {
        file.insert(rec);
    }

    // Mostrar todos los registros en orden
    cout << "Inorder Traversal of AVL Tree:" << endl;
    vector<Record> allRecords = file.inorder();
    for (auto& rec : allRecords) {
        rec.showData();
        cout << "---------------------" << endl;
    }

    // Buscar un registro específico
    int searchKey = 25;
    Record found = file.find(searchKey);
    cout << "Record Found: " << endl;
    found.showData();
}

extern "C" {
    Record* call_inorder(const char* filename, int* size) {
        AVLFile file(filename); // Usar el nombre del archivo proporcionado
        std::vector<Record> allRecords = file.inorder();
        
        // Asignar espacio para el arreglo
        *size = allRecords.size();
        Record* recordArray = new Record[*size];

        // Copiar los datos al arreglo
        for (int i = 0; i < *size; i++) {
            recordArray[i] = allRecords[i];
        }

        return recordArray; // Retornar puntero al arreglo
    }

    // Función para liberar memoria (importante)
    void free_records(Record* records) {
        delete[] records; // Liberar la memoria asignada
    }
}

int main() {
    //testAVLFile();
    return 0;
}

