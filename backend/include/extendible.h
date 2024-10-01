#ifndef Extendible
#define Extendible
#include <fstream>
#include <vector>
#include <string>
#include <functional> //hash function
#include <bitset>
#include <unordered_set>
#include "structures.h"
#include <set>

template <typename TK>
class extendible_hash{
private:
    void addRecord(TK &record, Bucket<TK> &bucketX, fstream &file, int &bucket);
    int add(TK &record, fstream &file);
    int search(TK &record, fstream &file, vector<TK> &result, int next); //Busca en un bucket
    int remove(TK &record, fstream &file, int prevBucket, int next);
    int deleteInBucket(Bucket<TK> &currentBucket, TK &record);
    int mergeBucket(Bucket<TK> &sourceBucket, Bucket<TK> &destinBucket);
    int writeBucket(Bucket<TK> &bucket, fstream &file, int pos);
    int readBucket(Bucket<TK> &bucket, fstream &file, int pos);


    int countRead;
    int countWrite;
    string filename, indexname;
    unordered_set<Index> indices;
    set<int> deletedBuckets;

    string hash_to_binary(TK record);
    void loadIndices();
    pair<string, int> getBucket(TK &record);
    void saveIndex();
    int validarBucket(const unordered_set<Index>& indices);
public:
    string indextype;
    extendible_hash(string _file, string _index, string _indextype);

    bool add(TK record);
    void splitIndex(unordered_set<Index>::const_iterator it, int bucket0, int bucket1);

    template <typename K>
    bool remove(const K& key);

    template <typename K>
    vector<TK> search(const K& key);

    void getData();
    void getIndices();
    void getDeletedBuckets();
    int getCountR();
    int getCountW();

    ~extendible_hash();

};

//Constructor
template <typename TK>
extendible_hash<TK>::extendible_hash(string _file, string _index, string _indexT): filename(_file), indexname(_index), indextype(_indexT), countRead(0), countWrite(0){
    this->indices = unordered_set<Index>();

    //Verifica que exista el file
    fstream file(this->filename, ios::in | ios::out | ios::binary);

    if(!file.is_open()){
        ofstream createFile(this->filename, ios::out | ios::binary);

        //Crear los primeros dos buckets vacíos
        file.seekg(0, ios::end);
        if (file.tellg() == 0){
            Bucket<TK> buck;
            file.write((char*) &buck, sizeof(Bucket<TK>));
            this->countWrite ++;
            file.write((char*) &buck, sizeof(Bucket<TK>));
            this->countWrite ++;
        }

        createFile.close();
    }
    file.close();

    //Verifica y crea el indexfile con los dos primeros valores
    fstream index(this->indexname, ios::in | ios::out | ios::binary);

    if(!index.is_open()){
        ofstream createFile(this->indexname, ios::out | ios::binary);
        Index index1("0", 0, 1);
        Index index2("1", 1, 1);
        createFile.write((char*) &index1, sizeof(Index));
        this->countWrite ++;
        createFile.write((char*) &index2, sizeof(Index));
        this->countWrite ++;
        createFile.close();
    }

    index.close();
    loadIndices();
}

//Funciones privadas
template <typename TK>
int extendible_hash<TK>::writeBucket(Bucket<TK> &bucket, fstream &file, int pos){
    file.seekp(pos*sizeof(Bucket<TK>), ios::beg);
    file.write((char*) &bucket, sizeof(Bucket<TK>));
    this->countWrite++;
    return 1;
}

template <typename TK>
int extendible_hash<TK>::readBucket(Bucket<TK> &bucket, fstream &file, int pos){
    file.seekg(pos*sizeof(Bucket<TK>), ios::beg);
    file.read((char*) &bucket, sizeof(Bucket<TK>));
    this->countRead++;
    return 1;
}

template <typename TK>
void extendible_hash<TK>::loadIndices(){
    fstream index(this->indexname, ios::in | ios::out | ios::binary);

    if (!index.is_open()) {cout << "Error opening index file: " << indexname << endl;return;}

    while (index.peek() != EOF) {
        Index idx;
        index.read((char*) &idx, sizeof(Index));
        this->countRead ++;
        if (index.gcount() == sizeof(Index)) {indices.insert(idx);}
    }

    index.close();
}

template <typename TK>
string extendible_hash<TK>::hash_to_binary(TK record){//CORREGIR
    size_t index;
    index = hash<string>()(record.Key());
    bitset<sizeof(size_t) * 8> binary(index);
    return binary.to_string();
}

template <typename TK>
int extendible_hash<TK>::validarBucket(const unordered_set<Index>& indices) { //NO SE USA temp
    int maxBucket = -1;
    for (const auto& index : indices) {
        if (index.bucket > maxBucket) {
            maxBucket = index.bucket;
        }
    }

    return maxBucket;
}
template <typename TK>
pair<string, int> extendible_hash<TK>::getBucket(TK &record){
    //Obtener codigo hash
    string code = hash_to_binary(record);
    //Obtener bucket al que pertenece
    int bucket = -1;
    string binario = "";
    int n = DEPTH;
    while(n >= 1){
        code = code.substr(code.size() - n);
        Index temp(code, -1,-1);
        auto it = indices.find(temp);
        if (it != indices.end()) {
            bucket = it->bucket;
            binario = it->binary;
            break;
        }
        n--;
    }
    //cout<<"BUCKET: "<<bucket<<" MAX Bucket: "<<validarBucket(indices)<<endl;

    return make_pair(binario, bucket);
}

template <typename TK>
void extendible_hash<TK>::saveIndex(){//OPTIMIZAR
    ofstream indexFile(this->indexname, ios::out | ios::binary );
    if (!indexFile.is_open()) {cout<< "Error opening index file for writing: " << indexname << endl;return;}

    for (const auto& index : indices) {
        indexFile.write((char*) (&index), sizeof(Index));
        this->countWrite ++;
    }
    indexFile.close();
}

template <typename TK>
void extendible_hash<TK>::addRecord(TK &record, Bucket<TK> &bucketX, fstream &file, int &bucket){
    //Función para agregar un elemento a un bucket con suficiente espacio
    bucketX.elements[bucketX.size] = record;
    bucketX.size += 1;
    writeBucket(bucketX, file, bucket);
}

template <typename TK>
void extendible_hash<TK>::splitIndex(unordered_set<Index>::const_iterator it, int bucket0, int bucket1){
    Index modifiedIndex0 = *it;
    Index modifiedIndex1 = *it;

    string nuevo0 = "0" + it->binary;
    string nuevo1 = "1" + it->binary;

    modifiedIndex0.Update(nuevo0, bucket0);
    modifiedIndex1.Update(nuevo1, bucket1);

    indices.erase(it);

    indices.insert(modifiedIndex0);
    indices.insert(modifiedIndex1);

    //cout<<"Nuevo Indice: "<<endl;
    //getIndices();

}

template <typename TK>
bool extendible_hash<TK>::add(TK record){
    fstream file(this->filename, ios::in | ios::out | ios::binary);
    if(!file.is_open()){return 0;}

    int result = this->add(record, file);

    file.close();
    return result;
}

template <typename TK>
int extendible_hash<TK>::add(TK &record, fstream &file){
    //Encontrar el bucket al que pertenece
    pair<string, int> indice = getBucket(record);
    int bucket = indice.second;
    if(bucket == -1) return 0;

    Bucket<TK> recordsBucket;
    readBucket(recordsBucket, file, bucket);
    file.clear();

    //cout<<"Elemento a insertar: "<<record<<endl;
    //cout<<"En el bucket: "<<bucket<<endl;
    string binario = indice.first;
    Index temp(binario, -1,-1);
    unordered_set<Index>::const_iterator it = indices.find(temp);

    if(recordsBucket.size < DF ){
        this->addRecord(record, recordsBucket, file, bucket);
    }else{//Split
        if(it->depth < DEPTH){
            //HacerSplit
            int bucket2;

            file.seekg(0, std::ios::end);
            bucket2 = file.tellg()/sizeof(Bucket<TK>);
            splitIndex(it, bucket, bucket2);

            //Reordenar el bucket traido en los dos obtenidos
            Bucket<TK> newBucket;
            writeBucket(newBucket, file, bucket2);

            cout<<"----"<<endl;

            TK players[DF+1];
            for (int i = 0; i < DF; ++i) players[i] = recordsBucket.elements[i];
            players[DF] = record;

            recordsBucket.size = 0;

            writeBucket(recordsBucket, file, bucket);

            for(int i = 0; i <DF+1; i++){
                add(players[i],file);
            }

        }else{
            while(recordsBucket.nextBucket >= 0 && recordsBucket.size == DF){
                bucket = recordsBucket.nextBucket;
                readBucket(recordsBucket, file, bucket);
            }
            //Overflow
            if(recordsBucket.nextBucket == -1 && recordsBucket.size == DF){
                Bucket<TK> overflowBucket;
                overflowBucket.elements[overflowBucket.size] = record;
                overflowBucket.size += 1;

                file.seekp(0, ios::end);
                file.write((char*) &overflowBucket, sizeof(Bucket<TK>));
                this->countWrite ++;

                //Establecer la posición del nuevo bucket
                file.seekg(0, ios::end);
                recordsBucket.nextBucket = (file.tellg()/sizeof(Bucket<TK>))-1;

                writeBucket(recordsBucket, file, bucket);
                return 1;

            }else{
                this->addRecord(record, recordsBucket, file, bucket);
            }
        }
    }

    return 1;
}

template <typename TK>
int extendible_hash<TK>::deleteInBucket(Bucket<TK> &currentBucket, TK &record){
    Bucket<TK> newBucket; //Bucket con los registros no eliminados

    for(auto play:currentBucket.elements) {
        if (play.Key() != record.Key()){
            newBucket.elements[newBucket.size] = play;
            newBucket.size +=1;
        }
    }
    //ModificarCurrentBucket
    currentBucket.size = newBucket.size;
    for (int i = 0; i < newBucket.size; i++) {
        currentBucket.elements[i] = newBucket.elements[i];
    }

}
template <typename TK>
int extendible_hash<TK>::mergeBucket(Bucket<TK> &sourceBucket, Bucket<TK> &destinBucket){
    for(int i=0; i<sourceBucket.size;i++){
        destinBucket.elements[destinBucket.size + i] = sourceBucket.elements[i];
    }
    destinBucket.size += sourceBucket.size;
}


template <typename TK>
int extendible_hash<TK>::remove(TK &record, fstream &file, int prevBucket, int next){
    int flag; //Bandera que me indica que debo modificar el índice, asignar un nuevo bucket
    int bucket;
    pair<string, int> indice = getBucket(record);
    if(prevBucket == -5) {
        flag = 1;
        bucket = indice.second;
    }else {
        bucket = next;
    }

    Bucket<TK> currentBucket;
    readBucket(currentBucket, file, bucket);

    currentBucket.showData();

    this->deleteInBucket(currentBucket, record); //currentBucket con todos los registros eliminados

    if(currentBucket.size <= DF/2){//Eliminar bucket
        string claveBin = indice.first;
        if(flag){//Significa que no hay prev, es decir su primera asignación viene del index
            //Si existe un nextbucket unir con ese (el next bucket pasa a ser la cabecera) si no existe verificar que
            //el prev prev tampoco tenga un next y recién hacer merge entre ellos y modificar el indice
            if(currentBucket.nextBucket != -1){
                cout<<"Primer caso, merge nextBUCK:"<<endl;
                Bucket<TK> nextBuck;
                readBucket(nextBuck, file,currentBucket.nextBucket);
                if(nextBuck.size <= DF/2){
                    mergeBucket(currentBucket, nextBuck);
                    //Actualizar mi índice para que apunte al nuevo bucket principal
                    Index temp(claveBin, -1,-1);

                    auto it = indices.find(temp);

                    Index actu(it->binary, currentBucket.nextBucket, it->depth);
                    indices.erase(it);
                    indices.insert(actu);
                    cout<<"Nuevo indice"<<endl;
                    getIndices();

                    this->writeBucket(nextBuck, file, currentBucket.nextBucket);
                    currentBucket.nextBucket = -1;
                    this->writeBucket(currentBucket, file, bucket);
                    //Agregar la posicion de currentBucket a mi directorio de eliminados
                    deletedBuckets.insert(bucket);
                }else{//Por muy pequeño que sea existe no más CAMBIAR LA CONIDICONAL PARA PODER METER BUCKETS MAS PEQUENIOS
                    this->writeBucket(currentBucket, file, bucket);
                }


            }else{//prev prev
                cout<<"clave bin original"<<claveBin<<endl;
                Index temp2(claveBin, -1,-1);
                if (claveBin[0] == '0') claveBin[0] = '1';
                else claveBin[0] = '0';

                Index temp(claveBin, -1,-1);
                unordered_set<Index>::const_iterator it = indices.find(temp);
                unordered_set<Index>::const_iterator it2 = indices.find(temp2);
                cout<<"clave bin hermana"<<it->binary<<endl;

                Bucket<TK> bucketHermano;
                readBucket(bucketHermano, file, it->bucket);

                //Verificar si el hermano no tiene next
                if(bucketHermano.nextBucket == -1){//MERGE DE AMBOS Y ALTERAR INDICE
                    mergeBucket(currentBucket, bucketHermano);
                    writeBucket(bucketHermano, file, it->bucket);
                    writeBucket(currentBucket, file, bucket); //suelto

                    //Actualizar indice
                    Index nuevo(claveBin, it->bucket,it->depth-1);
                    indices.insert(nuevo);
                    indices.erase(it);
                    indices.erase(it2);

                }else{//Por muy pequeño que sea existe no más
                    this->writeBucket(currentBucket, file, bucket);
                }

            }

        }else{//Significa que si hay prev
            Bucket<TK> preBucket;
            readBucket(preBucket, file, prevBucket);
            if(preBucket.size <= DF/2){
                mergeBucket(currentBucket, preBucket);
                if (currentBucket.nextBucket != -1) {
                    preBucket.nextBucket = currentBucket.nextBucket;
                    currentBucket.nextBucket = -1;
                }
                //Escribir los nuevos buckets en el archivo
                this->writeBucket(preBucket, file, prevBucket);
                this->writeBucket(currentBucket, file, bucket);
                //Agregar la posicion de currentBucket a mi directorio de eliminados
                deletedBuckets.insert(bucket);
            }else{
                if (currentBucket.nextBucket != -1){//Revisar que exista el nextBucket
                    Bucket<TK> nextBuck;
                    file.seekg(currentBucket.nextBucket*sizeof(Bucket<TK>), ios::beg);
                    file.read((char*) &nextBuck, sizeof(Bucket<TK>));
                    this->countRead++;

                    if(preBucket.size <= DF/2){ //Se desprende el bucket
                        mergeBucket(currentBucket, nextBuck);
                        preBucket.nextBucket = currentBucket.nextBucket;
                        currentBucket.nextBucket = -1;
                        //Escribir los nuevos buckets en el archivo
                        this->writeBucket(preBucket, file, prevBucket);
                        this->writeBucket(nextBuck, file, preBucket.nextBucket);
                        this->writeBucket(currentBucket, file, bucket);

                        //Agregar la posicion de currentBucket a mi directorio de eliminados
                        deletedBuckets.insert(bucket);
                    }else{//El bucket debe existir aunque sea pequeño
                        this->writeBucket(currentBucket, file, bucket);
                    }

                }else{//el bucket existe así sea pequeño
                    this->writeBucket(currentBucket, file, bucket);
                }
            }
        }
    }else{ //No afecta la eliminación del record
        writeBucket(currentBucket, file, bucket);
    }
    if(currentBucket.nextBucket != -1) remove(record, file, bucket, currentBucket.nextBucket);



}


template <typename TK>
template <typename K>
bool extendible_hash<TK>::remove(const K& key){
    TK record(key, indextype);
    fstream file(this->filename, ios::in | ios::out | ios::binary);
    if(!file.is_open()){return 0;}
    this->remove(record, file, -5, 0);

    file.close();
    return 1;
}

template <typename TK>
template <typename K>
vector<TK> extendible_hash<TK>::search(const K& key){
    vector<TK> result;
    TK record(key, indextype); // cambiar lo de cod, definir una variable privada que se inicializa con el constructor

    fstream file(this->filename, ios::in | ios::out | ios::binary);
    //if(!file.is_open()){return 0;}

    this->search(record, file, result, -5);

    file.close();

    return result;

}
template <typename TK>
int extendible_hash<TK>::search(TK &record, fstream &file, vector<TK> &result, int next){
    int bucket;
    if(next == -5){
        pair<string, int> indice = getBucket(record);
        bucket = indice.second;
    }else{
        bucket = next;
    }

    if(bucket == -1) return 0 ;

    Bucket<TK> currentBucket;
    readBucket(currentBucket, file, bucket);

    for(auto play:currentBucket.elements) {
        if (play.Key() == record.Key()){
            result.push_back(play);
        }
    }

    if(currentBucket.nextBucket == -1) return 0;
    else search(record, file, result, currentBucket.nextBucket);

    return 1;
}

template <typename TK>
void extendible_hash<TK>::getData(){
    fstream file(this->filename, ios::in | ios::out | ios::binary);

    int count = 0;
    while (file.peek() != EOF) {
        Bucket<TK> bp;
        file.read((char*) &bp, sizeof(Bucket<TK>));
        this->countRead++;
        //if(bp.size != 0){}
        cout<<"BUCKET #:"<<count<<endl;
        bp.showData();
        cout<<endl<<"NEXT: "<<bp.nextBucket<<endl;
        cout<<"SIZE: "<<bp.size<<endl;
        cout<<"---------------";

        count++;
        cout<<endl;
    }
    cout<<file.tellp()<<endl;

    file.close();
}

template <typename TK>
void extendible_hash<TK>::getIndices(){
    for(auto index:indices) cout<<index.binary<<"-"<<index.bucket<<"-"<<index.depth<<endl;
}

template <typename TK>
void extendible_hash<TK>::getDeletedBuckets(){
    for(auto deleted: deletedBuckets) cout<<deleted<<endl;
}

template <typename TK>
int extendible_hash<TK>::getCountR(){return this->countRead;}

template <typename TK>
int extendible_hash<TK>::getCountW(){return this->countWrite;}

template <typename TK>
extendible_hash<TK>::~extendible_hash(){this->saveIndex();}

#endif