#ifndef EMPLOYEE_EH
#define EMPLOYEE_EH
#include "employee.h"
#include <strstream>
using namespace std;


template<typename TK>
class EmployeeEH:public EmployeeRecord{
public:
    EmployeeEH() : EmployeeRecord(){}

    EmployeeEH(int _key, const char* _FirstName, const char* _LastName, const char* _StartDate, const char* _ExitDate,
               const char* _Title, const char* _Supervisor, const char* _ADEmail, const char* _BusinessUnit,
               const char* _EmployeeStatus, const char* _EmployeeType, const char* _PayZone,
               const char* _EmployeeClassificationType, const char* _TerminationType,
               const char* _TerminationDescription, const char* _DepartmentType, const char* _Division,
               const char* _DOB, const char* _State, const char* _JobFunctionDescription, const char* _GenderCode,
               const char* _LocationCode, const char* _RaceDesc, const char* _MaritalDesc, const char* _PerformanceScore,
               double _CurrentEmployeeRating)
            : EmployeeRecord() {
        this->key = _key;
        strcpy(this->FirstName, _FirstName);
        strcpy(this->LastName, _LastName);
        strcpy(this->StartDate, _StartDate);
        strcpy(this->ExitDate, _ExitDate);
        strcpy(this->Title, _Title);
        strcpy(this->Supervisor, _Supervisor);
        strcpy(this->ADEmail, _ADEmail);
        strcpy(this->BusinessUnit, _BusinessUnit);
        strcpy(this->EmployeeStatus, _EmployeeStatus);
        strcpy(this->EmployeeType, _EmployeeType);
        strcpy(this->PayZone, _PayZone);
        strcpy(this->EmployeeClassificationType, _EmployeeClassificationType);
        strcpy(this->TerminationType, _TerminationType);
        strcpy(this->TerminationDescription, _TerminationDescription);
        strcpy(this->DepartmentType, _DepartmentType);
        strcpy(this->Division, _Division);
        strcpy(this->DOB, _DOB);
        strcpy(this->State, _State);
        strcpy(this->JobFunctionDescription, _JobFunctionDescription);
        strcpy(this->GenderCode, _GenderCode);
        strcpy(this->LocationCode, _LocationCode);
        strcpy(this->RaceDesc, _RaceDesc);
        strcpy(this->MaritalDesc, _MaritalDesc);
        strcpy(this->PerformanceScore, _PerformanceScore);
        this->CurrentEmployeeRating = _CurrentEmployeeRating;
    }
    EmployeeEH(TK key, string keyType){//CONSTRUCTOR PARA EXTENDER A MAS tipos de keys
        if(keyType == "EmpID"){
            this->key = key;
            strcpy(this->FirstName, "");
            strcpy(this->LastName, "");
            strcpy(this->StartDate, "");
            strcpy(this->ExitDate, "");
            strcpy(this->Title, "");
            strcpy(this->Supervisor, "");
            strcpy(this->ADEmail, "");
            strcpy(this->BusinessUnit, "");
            strcpy(this->EmployeeStatus, "");
            strcpy(this->EmployeeType, "");
            strcpy(this->PayZone, "");
            strcpy(this->EmployeeClassificationType, "");
            strcpy(this->TerminationType, "");
            strcpy(this->TerminationDescription, "");
            strcpy(this->DepartmentType, "");
            strcpy(this->Division, "");
            strcpy(this->DOB, "");
            strcpy(this->State, "");
            strcpy(this->JobFunctionDescription, "");
            strcpy(this->GenderCode, "");
            strcpy(this->LocationCode, "");
            strcpy(this->RaceDesc, "");
            strcpy(this->MaritalDesc, "");
            strcpy(this->PerformanceScore, "");
            this->CurrentEmployeeRating = 0.0;
        }else if(keyType == "FirstName"){
                this->key = 0;
                strcpy(this->FirstName, key);
                strcpy(this->LastName, "");
                strcpy(this->StartDate, "");
                strcpy(this->ExitDate, "");
                strcpy(this->Title, "");
                strcpy(this->Supervisor, "");
                strcpy(this->ADEmail, "");
                strcpy(this->BusinessUnit, "");
                strcpy(this->EmployeeStatus, "");
                strcpy(this->EmployeeType, "");
                strcpy(this->PayZone, "");
                strcpy(this->EmployeeClassificationType, "");
                strcpy(this->TerminationType, "");
                strcpy(this->TerminationDescription, "");
                strcpy(this->DepartmentType, "");
                strcpy(this->Division, "");
                strcpy(this->DOB, "");
                strcpy(this->State, "");
                strcpy(this->JobFunctionDescription, "");
                strcpy(this->GenderCode, "");
                strcpy(this->LocationCode, "");
                strcpy(this->RaceDesc, "");
                strcpy(this->MaritalDesc, "");
                strcpy(this->PerformanceScore, "");
                this->CurrentEmployeeRating = 0.0;
        }
    }


    string Key() const{
        ostrstream key;
        key<<this->key<<ends;
        return key.str();
    }
    void showData() const {
        cout << "\nEmpID: " << key;
        cout << "\nFirstName: " << FirstName;
        cout << "\nLastName: " << LastName;
        cout << "\nStartDate: " << StartDate;
        cout << "\nExitDate: " << ExitDate;
        cout << "\nTitle: " << Title;
        cout << "\nSupervisor: " << Supervisor;
        cout << "\nADEmail: " << ADEmail;
        cout << "\nBusinessUnit: " << BusinessUnit;
        cout << "\nEmployeeStatus: " << EmployeeStatus;
        cout << "\nEmployeeType: " << EmployeeType;
        cout << "\nPayZone: " << PayZone;
        cout << "\nEmployeeClassificationType: " << EmployeeClassificationType;
        cout << "\nTerminationType: " << TerminationType;
        cout << "\nTerminationDescription: " << TerminationDescription;
        cout << "\nDepartmentType: " << DepartmentType;
        cout << "\nDivision: " << Division;
        cout << "\nDOB: " << DOB;
        cout << "\nState: " << State;
        cout << "\nJobFunctionDescription: " << JobFunctionDescription;
        cout << "\nGenderCode: " << GenderCode;
        cout << "\nLocationCode: " << LocationCode;
        cout << "\nRaceDesc: " << RaceDesc;
        cout << "\nMaritalDesc: " << MaritalDesc;
        cout << "\nPerformanceScore: " << PerformanceScore;
        cout << "\nCurrentEmployeeRating: " << CurrentEmployeeRating;

    }
    EmployeeEH& operator=(const EmployeeEH& other) {
        if (this != &other) {
            this->key = other.key;
            strncpy(this->FirstName, other.FirstName, sizeof(this->FirstName) - 1);
            this->FirstName[sizeof(this->FirstName) - 1] = '\0';

            strncpy(this->LastName, other.LastName, sizeof(this->LastName) - 1);
            this->LastName[sizeof(this->LastName) - 1] = '\0';

            strncpy(this->StartDate, other.StartDate, sizeof(this->StartDate) - 1);
            this->StartDate[sizeof(this->StartDate) - 1] = '\0';

            strncpy(this->ExitDate, other.ExitDate, sizeof(this->ExitDate) - 1);
            this->ExitDate[sizeof(this->ExitDate) - 1] = '\0';

            strncpy(this->Title, other.Title, sizeof(this->Title) - 1);
            this->Title[sizeof(this->Title) - 1] = '\0';

            strncpy(this->Supervisor, other.Supervisor, sizeof(this->Supervisor) - 1);
            this->Supervisor[sizeof(this->Supervisor) - 1] = '\0';

            strncpy(this->ADEmail, other.ADEmail, sizeof(this->ADEmail) - 1);
            this->ADEmail[sizeof(this->ADEmail) - 1] = '\0';

            strncpy(this->BusinessUnit, other.BusinessUnit, sizeof(this->BusinessUnit) - 1);
            this->BusinessUnit[sizeof(this->BusinessUnit) - 1] = '\0';

            strncpy(this->EmployeeStatus, other.EmployeeStatus, sizeof(this->EmployeeStatus) - 1);
            this->EmployeeStatus[sizeof(this->EmployeeStatus) - 1] = '\0';

            strncpy(this->EmployeeType, other.EmployeeType, sizeof(this->EmployeeType) - 1);
            this->EmployeeType[sizeof(this->EmployeeType) - 1] = '\0';

            strncpy(this->PayZone, other.PayZone, sizeof(this->PayZone) - 1);
            this->PayZone[sizeof(this->PayZone) - 1] = '\0';

            strncpy(this->EmployeeClassificationType, other.EmployeeClassificationType, sizeof(this->EmployeeClassificationType) - 1);
            this->EmployeeClassificationType[sizeof(this->EmployeeClassificationType) - 1] = '\0';

            strncpy(this->TerminationType, other.TerminationType, sizeof(this->TerminationType) - 1);
            this->TerminationType[sizeof(this->TerminationType) - 1] = '\0';

            strncpy(this->TerminationDescription, other.TerminationDescription, sizeof(this->TerminationDescription) - 1);
            this->TerminationDescription[sizeof(this->TerminationDescription) - 1] = '\0';

            strncpy(this->DepartmentType, other.DepartmentType, sizeof(this->DepartmentType) - 1);
            this->DepartmentType[sizeof(this->DepartmentType) - 1] = '\0';

            strncpy(this->Division, other.Division, sizeof(this->Division) - 1);
            this->Division[sizeof(this->Division) - 1] = '\0';

            strncpy(this->DOB, other.DOB, sizeof(this->DOB) - 1);
            this->DOB[sizeof(this->DOB) - 1] = '\0';

            strncpy(this->State, other.State, sizeof(this->State) - 1);
            this->State[sizeof(this->State) - 1] = '\0';

            strncpy(this->JobFunctionDescription, other.JobFunctionDescription, sizeof(this->JobFunctionDescription) - 1);
            this->JobFunctionDescription[sizeof(this->JobFunctionDescription) - 1] = '\0';

            strncpy(this->GenderCode, other.GenderCode, sizeof(this->GenderCode) - 1);
            this->GenderCode[sizeof(this->GenderCode) - 1] = '\0';

            strncpy(this->LocationCode, other.LocationCode, sizeof(this->LocationCode) - 1);
            this->LocationCode[sizeof(this->LocationCode) - 1] = '\0';

            strncpy(this->RaceDesc, other.RaceDesc, sizeof(this->RaceDesc) - 1);
            this->RaceDesc[sizeof(this->RaceDesc) - 1] = '\0';

            strncpy(this->MaritalDesc, other.MaritalDesc, sizeof(this->MaritalDesc) - 1);
            this->MaritalDesc[sizeof(this->MaritalDesc) - 1] = '\0';

            strncpy(this->PerformanceScore, other.PerformanceScore, sizeof(this->PerformanceScore) - 1);
            this->PerformanceScore[sizeof(this->PerformanceScore) - 1] = '\0';

            this->CurrentEmployeeRating = other.CurrentEmployeeRating;
        }
        return *this;
    }

};

template <typename K>
ostream& operator<<(ostream& os, const EmployeeEH<K>& record) {
    os << "EmpID: " << record.key << "\n";
    os << "FirstName: " << record.FirstName << "\n";
    os << "LastName: " << record.LastName << "\n";
    os << "StartDate: " << record.StartDate << "\n";
    os << "ExitDate: " << record.ExitDate << "\n";
    os << "Title: " << record.Title << "\n";
    os << "Supervisor: " << record.Supervisor << "\n";
    os << "ADEmail: " << record.ADEmail << "\n";
    os << "BusinessUnit: " << record.BusinessUnit << "\n";
    os << "EmployeeStatus: " << record.EmployeeStatus << "\n";
    os << "EmployeeType: " << record.EmployeeType << "\n";
    os << "PayZone: " << record.PayZone << "\n";
    os << "EmployeeClassificationType: " << record.EmployeeClassificationType << "\n";
    os << "TerminationType: " << record.TerminationType << "\n";
    os << "TerminationDescription: " << record.TerminationDescription << "\n";
    os << "DepartmentType: " << record.DepartmentType << "\n";
    os << "Division: " << record.Division << "\n";
    os << "DOB: " << record.DOB << "\n";
    os << "State: " << record.State << "\n";
    os << "JobFunctionDescription: " << record.JobFunctionDescription << "\n";
    os << "GenderCode: " << record.GenderCode << "\n";
    os << "LocationCode: " << record.LocationCode << "\n";
    os << "RaceDesc: " << record.RaceDesc << "\n";
    os << "MaritalDesc: " << record.MaritalDesc << "\n";
    os << "PerformanceScore: " << record.PerformanceScore << "\n";
    os << "CurrentEmployeeRating: " << record.CurrentEmployeeRating << "\n";
    return os;
}


#endif 
