#ifndef EH_STRUCTURE
#define EH_STRUCTURE
#include <iostream>
#include <cstring>
#include <strstream>
using namespace std;

#define DF 12 //Cantidad de elementos bucket
const int DEPTH = 8; //binary

//BUCKET
template<typename T>
struct Bucket{
    T elements[DF];
    int size;
    int nextBucket;
    Bucket():size(0), nextBucket(-1){
        /*for (int i = 0; i < DF; ++i) {
            new (&elements[i]) T();
        }*/
    }
    void showData(){
        for(int i = 0; i < size; i++){
            elements[i].showData();
        }
    }
    ~Bucket(){}

};

//INDICE
struct Index{
    string binary;
    int bucket;
    int depth;
    Index(): binary(""), bucket(-1), depth(-1) {}

    Index(string _binary, int _b, int _d){
        this->binary = _binary;
        this->bucket = _b;
        this->depth = _d;
    }

    bool operator==(const Index& other) const {
        return binary == other.binary; //&& bucket == other.bucket && depth == depth;
    }

    void Update(string _binary, int _bucket) {
        this->binary = _binary;
        this->bucket = _bucket;
        this->depth = depth + 1;
    }
};

namespace std {
    template <>
    struct hash<Index> {
        size_t operator()(const Index& index) const {
            return hash<string>()(index.binary);
        }
    };
}

#endif //EXTENDIBLE_HASHING_STRUCTURES_H
