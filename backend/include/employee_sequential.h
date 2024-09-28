#ifndef EMPLOYEE_SEQUENTIAL
#define EMPLOYEE_SEQUENTIAL

#include "employee.h"

class EmployeeRecordSequential : public EmployeeRecord{
public:
    int nextRecord;        // Puntero al siguiente registro
    char nextFileChar;     // Indica el archivo siguiente
    char accFile;         // Archivo de acceso
    int pos;              // Posición del registro

    // Constructor por defecto
    EmployeeRecordSequential() : EmployeeRecord(), nextRecord(-1), nextFileChar(' '), accFile(' '), pos(-1) {}
};

#endif // EMPLOYEE_SEQUENTIAL

