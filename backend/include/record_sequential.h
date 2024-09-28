#ifndef RECORD_SEQUENTIAL_H
#define RECORD_SEQUENTIAL_H

#include <cstring>

struct Record_S {
    int key;
    char data[20];
    int nextRecord;
    char nextFileChar;
    char accFile;
    int pos;

    // Constructor por defecto
    Record_S() : key(-1), nextRecord(-1), nextFileChar('d'), accFile('a'), pos(0) {
        strcpy(data, "Dummy");
    }

    // Constructor que recibe key y data, dejando los demás valores por defecto
    Record_S(int k, const char* d) : key(k), nextRecord(-1), nextFileChar('d'), accFile('a'), pos(0) {
        strncpy(data, d, sizeof(data) - 1);
        data[sizeof(data) - 1] = '\0';  // Asegurar que esté terminado en null
    }
};

#endif // RECORD_SEQUENTIAL_H

