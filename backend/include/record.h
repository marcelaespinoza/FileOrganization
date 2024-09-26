#ifndef RECORD_H
#define RECORD_H

#include <iostream>
#include <cstring>

using namespace std;

struct Record {
    int cod; // Clave primaria (4 bytes)
    char nombre[12]; // Nombre (12 bytes)
    int ciclo; // Ciclo (4 bytes)

    long left, right; // Punteros a los hijos izquierdo y derecho
    int height; // Altura del nodo

    Record() : cod(0), ciclo(0), left(-1), right(-1), height(1) {}

    Record(int cod, const char* nombre, int ciclo)
        : cod(cod), ciclo(ciclo), left(-1), right(-1), height(1) {
        strncpy(this->nombre, nombre, sizeof(this->nombre) - 1);
        this->nombre[sizeof(this->nombre) - 1] = '\0';
    }

    void showData() const {
        cout << "\nCodigo: " << cod;
        cout << "\nNombre: " << nombre;
        cout << "\nCiclo: " << ciclo;
        cout << "\nLeft: " << left;
        cout << "\nRight: " << right;
        cout << "\nHeight: " << height << endl;
    }
};

#endif // RECORD_H

