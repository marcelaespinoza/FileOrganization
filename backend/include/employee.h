#ifndef EMPLOYEE
#define EMPLOYEE

#include <iostream>

struct EmployeeRecord {
    int key; //key 
    char FirstName[250];                   // Nombre
    char LastName[250];                    // Apellido
    char StartDate[250];                   // Fecha de inicio (formato yyyy-mm-dd)
    char ExitDate[250];                    // Fecha de salida (formato yyyy-mm-dd)
    char Title[250];                       // Título o puesto
    char Supervisor[250];                  // Supervisor del empleado
    char ADEmail[250];                     // Correo electrónico
    char BusinessUnit[250];                // Unidad de negocio
    char EmployeeStatus[250];              // Estado del empleado (e.g., Activo, Inactivo)
    char EmployeeType[250];                // Tipo de empleado (e.g., Tiempo completo, Medio tiempo)
    char PayZone[250];                     // Zona de pago
    char EmployeeClassificationType[250];  // Tipo de clasificación del empleado
    char TerminationType[250];             // Tipo de terminación (si aplica)
    char TerminationDescription[250];      // Descripción de la terminación (si aplica)
    char DepartmentType[250];              // Tipo de departamento
    char Division[250];                    // División del empleado
    char DOB[250];                         // Fecha de nacimiento (formato yyyy-mm-dd)
    char State[250];                       // Estado (región)
    char JobFunctionDescription[250];      // Descripción de la función del trabajo
    char GenderCode[250];                  // Código de género (e.g., M/F/O)
    char LocationCode[250];                // Código de ubicación
    char RaceDesc[250];                    // Descripción de raza/etnicidad
    char MaritalDesc[250];                 // Descripción del estado civil
    char PerformanceScore[250];            // Desempeño
    double CurrentEmployeeRating;          // Calificación actual del empleado

    // Constructor por defecto
    EmployeeRecord() : key(-1), CurrentEmployeeRating(0.0) {
        // Inicializar los campos a cadenas vacías
        FirstName[0] = '\0';
        LastName[0] = '\0';
        StartDate[0] = '\0';
        ExitDate[0] = '\0';
        Title[0] = '\0';
        Supervisor[0] = '\0';
        ADEmail[0] = '\0';
        BusinessUnit[0] = '\0';
        EmployeeStatus[0] = '\0';
        EmployeeType[0] = '\0';
        PayZone[0] = '\0';
        EmployeeClassificationType[0] = '\0';
        TerminationType[0] = '\0';
        TerminationDescription[0] = '\0';
        DepartmentType[0] = '\0';
        Division[0] = '\0';
        DOB[0] = '\0';
        State[0] = '\0';
        JobFunctionDescription[0] = '\0';
        GenderCode[0] = '\0';
        LocationCode[0] = '\0';
        RaceDesc[0] = '\0';
        MaritalDesc[0] = '\0';
        PerformanceScore[0] = '\0';
    }
};

#endif // EMPLOYEE

