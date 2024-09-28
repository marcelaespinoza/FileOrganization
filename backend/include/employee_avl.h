#ifndef EMPLOYEE_AVL
#define EMPLOYEE_AVL

#include "employee.h"

class EmployeeRecordAVL : public EmployeeRecord{
public:
    long left; // Puntero al hijo izquierdo
    long right; // Puntero al hijo derecho
    int height; // Altura del nodo

    // Constructor por defecto
    EmployeeRecordAVL() : EmployeeRecord(), left(-1), right(-1), height(1) {}
};

#endif // EMPLOYEE_AVL

