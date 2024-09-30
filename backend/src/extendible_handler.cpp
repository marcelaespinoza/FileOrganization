#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <cpprest/uri.h>
#include "../include/extendible.h"
#include "../include/games_EH.h"
#include "../include/employee_EH.h"
#include "../include/utils.h"
#include <string>
#include <vector>

using namespace web;
using namespace http;

class ExtendibleHandler {
public:
    void get_all(http_request request, const uri& uri) {
        auto query = uri::split_query(uri.query());
        if (query.find(U("table_name")) != query.end()) {
            std::string table_name = utility::conversions::to_utf8string(query[U("table_name")]);
            if(table_name == "employee"){
              AVLFile<int, EmployeeRecordAVL> avlFile(table_name+"_avl.dat");
              int reads = 0; 
              int writes = 0;
              auto inorderResult = avlFile.inorder(reads, writes);
              
              json::value response_data = json::value::array();
              for (size_t i = 0; i < inorderResult.size(); ++i) {
                  json::value recordJson;
                  recordJson[U("key")] = web::json::value::number(inorderResult[i].key);
                  recordJson[U("FirstName")] = web::json::value::string(utility::conversions::to_string_t(inorderResult[i].FirstName));
                  recordJson[U("LastName")] = web::json::value::string(utility::conversions::to_string_t(inorderResult[i].LastName));
                  recordJson[U("StartDate")] = web::json::value::string(utility::conversions::to_string_t(inorderResult[i].StartDate));
                  recordJson[U("ExitDate")] = web::json::value::string(utility::conversions::to_string_t(inorderResult[i].ExitDate));
                  recordJson[U("Title")] = web::json::value::string(utility::conversions::to_string_t(inorderResult[i].Title));
                  recordJson[U("Supervisor")] = web::json::value::string(utility::conversions::to_string_t(inorderResult[i].Supervisor));
                  recordJson[U("ADEmail")] = web::json::value::string(utility::conversions::to_string_t(inorderResult[i].ADEmail));
                  recordJson[U("BusinessUnit")] = web::json::value::string(utility::conversions::to_string_t(inorderResult[i].BusinessUnit));
                  recordJson[U("EmployeeStatus")] = web::json::value::string(utility::conversions::to_string_t(inorderResult[i].EmployeeStatus));
                  recordJson[U("EmployeeType")] = web::json::value::string(utility::conversions::to_string_t(inorderResult[i].EmployeeType));
                  recordJson[U("PayZone")] = web::json::value::string(utility::conversions::to_string_t(inorderResult[i].PayZone));
                  recordJson[U("EmployeeClassificationType")] = web::json::value::string(utility::conversions::to_string_t(inorderResult[i].EmployeeClassificationType));
                  recordJson[U("TerminationType")] = web::json::value::string(utility::conversions::to_string_t(inorderResult[i].TerminationType));
                  recordJson[U("TerminationDescription")] = web::json::value::string(utility::conversions::to_string_t(inorderResult[i].TerminationDescription));
                  recordJson[U("DepartmentType")] = web::json::value::string(utility::conversions::to_string_t(inorderResult[i].DepartmentType));
                  recordJson[U("Division")] = web::json::value::string(utility::conversions::to_string_t(inorderResult[i].Division));
                  recordJson[U("DOB")] = web::json::value::string(utility::conversions::to_string_t(inorderResult[i].DOB));
                  recordJson[U("State")] = web::json::value::string(utility::conversions::to_string_t(inorderResult[i].State));
                  recordJson[U("JobFunctionDescription")] = web::json::value::string(utility::conversions::to_string_t(inorderResult[i].JobFunctionDescription));
                  recordJson[U("GenderCode")] = web::json::value::string(utility::conversions::to_string_t(inorderResult[i].GenderCode));
                  recordJson[U("LocationCode")] = web::json::value::string(utility::conversions::to_string_t(inorderResult[i].LocationCode));
                  recordJson[U("RaceDesc")] = web::json::value::string(utility::conversions::to_string_t(inorderResult[i].RaceDesc));
                  recordJson[U("MaritalDesc")] = web::json::value::string(utility::conversions::to_string_t(inorderResult[i].MaritalDesc));
                  recordJson[U("PerformanceScore")] = web::json::value::string(utility::conversions::to_string_t(inorderResult[i].PerformanceScore));
                  recordJson[U("CurrentEmployeeRating")] = web::json::value::number(inorderResult[i].CurrentEmployeeRating);
                  response_data[i] = recordJson;
              }
              json::value responseJson;
              responseJson[U("reads")] = json::value::number(reads);
              responseJson[U("writes")] = json::value::number(writes);
              responseJson[U("results")] = response_data;
              request.reply(status_codes::OK, responseJson);
            }
            if(table_name == "game"){
              AVLFile<int, GameRecordAVL> avlFile(table_name+"_avl.dat");
              int reads = 0; 
              int writes = 0;
              auto inorderResult = avlFile.inorder(reads, writes);
              json::value response_data = json::value::array();
              for (size_t i = 0; i < inorderResult.size(); ++i) {
                  json::value recordJson;
                  recordJson[U("key")] = web::json::value::number(inorderResult[i].key);
                  recordJson[U("Rank")] = web::json::value::number(inorderResult[i].Rank);
                  recordJson[U("GameTitle")] = web::json::value::string(utility::conversions::to_string_t(inorderResult[i].GameTitle));
                  recordJson[U("Platform")] = web::json::value::string(utility::conversions::to_string_t(inorderResult[i].Platform));
                  recordJson[U("Year")] = web::json::value::number(inorderResult[i].Year);
                  recordJson[U("Genre")] = web::json::value::string(utility::conversions::to_string_t(inorderResult[i].Genre));
                  recordJson[U("Publisher")] = web::json::value::string(utility::conversions::to_string_t(inorderResult[i].Publisher));
                  recordJson[U("NorthAmerica")] = web::json::value::number(inorderResult[i].NorthAmerica);
                  recordJson[U("Europe")] = web::json::value::number(inorderResult[i].Europe);
                  recordJson[U("Japan")] = web::json::value::number(inorderResult[i].Japan);
                  recordJson[U("RestOfWorld")] = web::json::value::number(inorderResult[i].RestOfWorld);
                  recordJson[U("Global")] = web::json::value::number(inorderResult[i].Global);
                  recordJson[U("Review")] = web::json::value::number(inorderResult[i].Review);
                  response_data[i] = recordJson;
              }
              json::value responseJson;
              responseJson[U("reads")] = json::value::number(reads);
              responseJson[U("writes")] = json::value::number(writes);
              responseJson[U("results")] = response_data;
              request.reply(status_codes::OK, responseJson);
            }
        } else {
            request.reply(status_codes::BadRequest, U("Filename parameter is required."));
        }
    }

    void get_record(http_request request, const uri& uri) {
        auto query = uri::split_query(uri.query());
        if (query.find(U("key")) != query.end() && query.find(U("table_name")) != query.end()) {
            int key = std::stoi(utility::conversions::to_utf8string(query[U("key")]));
            std::string table_name = utility::conversions::to_utf8string(query[U("table_name")]);
            if (table_name == "employee"){
              AVLFile<int, EmployeeRecordAVL> avlFile(table_name+"_avl.dat");
              int reads = 0; 
              int writes = 0;
              EmployeeRecordAVL record = avlFile.search_by_key(key, reads, writes);

              if (record.key != 1) {
                  json::value recordJson;
                  recordJson[U("key")] = web::json::value::number(record.key);
                  recordJson[U("FirstName")] = web::json::value::string(utility::conversions::to_string_t(record.FirstName));
                  recordJson[U("LastName")] = web::json::value::string(utility::conversions::to_string_t(record.LastName));
                  recordJson[U("StartDate")] = web::json::value::string(utility::conversions::to_string_t(record.StartDate));
                  recordJson[U("ExitDate")] = web::json::value::string(utility::conversions::to_string_t(record.ExitDate));
                  recordJson[U("Title")] = web::json::value::string(utility::conversions::to_string_t(record.Title));
                  recordJson[U("Supervisor")] = web::json::value::string(utility::conversions::to_string_t(record.Supervisor));
                  recordJson[U("ADEmail")] = web::json::value::string(utility::conversions::to_string_t(record.ADEmail));
                  recordJson[U("BusinessUnit")] = web::json::value::string(utility::conversions::to_string_t(record.BusinessUnit));
                  recordJson[U("EmployeeStatus")] = web::json::value::string(utility::conversions::to_string_t(record.EmployeeStatus));
                  recordJson[U("EmployeeType")] = web::json::value::string(utility::conversions::to_string_t(record.EmployeeType));
                  recordJson[U("PayZone")] = web::json::value::string(utility::conversions::to_string_t(record.PayZone));
                  recordJson[U("EmployeeClassificationType")] = web::json::value::string(utility::conversions::to_string_t(record.EmployeeClassificationType));
                  recordJson[U("TerminationType")] = web::json::value::string(utility::conversions::to_string_t(record.TerminationType));
                  recordJson[U("TerminationDescription")] = web::json::value::string(utility::conversions::to_string_t(record.TerminationDescription));
                  recordJson[U("DepartmentType")] = web::json::value::string(utility::conversions::to_string_t(record.DepartmentType));
                  recordJson[U("Division")] = web::json::value::string(utility::conversions::to_string_t(record.Division));
                  recordJson[U("DOB")] = web::json::value::string(utility::conversions::to_string_t(record.DOB));
                  recordJson[U("State")] = web::json::value::string(utility::conversions::to_string_t(record.State));
                  recordJson[U("JobFunctionDescription")] = web::json::value::string(utility::conversions::to_string_t(record.JobFunctionDescription));
                  recordJson[U("GenderCode")] = web::json::value::string(utility::conversions::to_string_t(record.GenderCode));
                  recordJson[U("LocationCode")] = web::json::value::string(utility::conversions::to_string_t(record.LocationCode));
                  recordJson[U("RaceDesc")] = web::json::value::string(utility::conversions::to_string_t(record.RaceDesc));
                  recordJson[U("MaritalDesc")] = web::json::value::string(utility::conversions::to_string_t(record.MaritalDesc));
                  recordJson[U("PerformanceScore")] = web::json::value::string(utility::conversions::to_string_t(record.PerformanceScore));
                  recordJson[U("CurrentEmployeeRating")] = web::json::value::number(record.CurrentEmployeeRating);

                  json::value responseJson;
                  responseJson[U("reads")] = json::value::number(reads);
                  responseJson[U("writes")] = json::value::number(writes);
                  responseJson[U("results")] = recordJson;
                  request.reply(status_codes::OK, responseJson);
              } else {
                  request.reply(status_codes::NotFound, U("Record not found."));
              }
            }
            if (table_name == "game"){
              AVLFile<int, GameRecordAVL> avlFile(table_name+"_avl.dat");
              int reads = 0;
              int writes = 0;
              GameRecordAVL record = avlFile.search_by_key(key, reads, writes);
              if (record.key != -1) {
                  json::value recordJson;
                  recordJson[U("key")] = web::json::value::number(record.key);
                  recordJson[U("Rank")] = web::json::value::number(record.Rank);
                  recordJson[U("GameTitle")] = web::json::value::string(utility::conversions::to_string_t(record.GameTitle));
                  recordJson[U("Platform")] = web::json::value::string(utility::conversions::to_string_t(record.Platform));
                  recordJson[U("Year")] = web::json::value::number(record.Year);
                  recordJson[U("Genre")] = web::json::value::string(utility::conversions::to_string_t(record.Genre));
                  recordJson[U("Publisher")] = web::json::value::string(utility::conversions::to_string_t(record.Publisher));
                  recordJson[U("NorthAmerica")] = web::json::value::number(record.NorthAmerica);
                  recordJson[U("Europe")] = web::json::value::number(record.Europe);
                  recordJson[U("Japan")] = web::json::value::number(record.Japan);
                  recordJson[U("RestOfWorld")] = web::json::value::number(record.RestOfWorld);
                  recordJson[U("Global")] = web::json::value::number(record.Global);
                  recordJson[U("Review")] = web::json::value::number(record.Review);

                  json::value responseJson;
                  responseJson[U("reads")] = json::value::number(reads);
                  responseJson[U("writes")] = json::value::number(writes);
                  responseJson[U("results")] = recordJson;
                  request.reply(status_codes::OK, responseJson);
              } else {
                  request.reply(status_codes::NotFound, U("Record not found."));
              }
            }

        } else {
            request.reply(status_codes::BadRequest, U("Key and filename parameters are required."));
        }
    }

   
    void post_record(http_request request, const uri& uri) {
        auto query = uri::split_query(uri.query());
        if (query.find(U("table_name")) != query.end()) {
            std::string table_name = utility::conversions::to_utf8string(query[U("table_name")]);
            if(table_name == "employee"){
                request.extract_json().then([=](json::value request_data) {
                if (request_data.has_field(U("key")) && 
                    request_data.has_field(U("FirstName")) && 
                    request_data.has_field(U("LastName")) && 
                    request_data.has_field(U("StartDate")) && 
                    request_data.has_field(U("ExitDate")) && 
                    request_data.has_field(U("Title")) && 
                    request_data.has_field(U("Supervisor")) && 
                    request_data.has_field(U("ADEmail")) && 
                    request_data.has_field(U("BusinessUnit")) && 
                    request_data.has_field(U("EmployeeStatus")) && 
                    request_data.has_field(U("EmployeeType")) && 
                    request_data.has_field(U("PayZone")) && 
                    request_data.has_field(U("EmployeeClassificationType")) && 
                    request_data.has_field(U("TerminationType")) && 
                    request_data.has_field(U("TerminationDescription")) && 
                    request_data.has_field(U("DepartmentType")) && 
                    request_data.has_field(U("Division")) && 
                    request_data.has_field(U("DOB")) && 
                    request_data.has_field(U("State")) && 
                    request_data.has_field(U("JobFunctionDescription")) && 
                    request_data.has_field(U("GenderCode")) && 
                    request_data.has_field(U("LocationCode")) && 
                    request_data.has_field(U("RaceDesc")) && 
                    request_data.has_field(U("MaritalDesc")) && 
                    request_data.has_field(U("PerformanceScore")) && 
                    request_data.has_field(U("CurrentEmployeeRating"))) {
                    EmployeeRecordAVL record;
                    record.key = request_data[U("key")].as_integer();
                    strncpy(record.FirstName, utility::conversions::to_utf8string(request_data[U("FirstName")].as_string()).c_str(), sizeof(record.FirstName) - 1);
                    record.FirstName[sizeof(record.FirstName) - 1] = '\0'; 
                    strncpy(record.LastName, utility::conversions::to_utf8string(request_data[U("LastName")].as_string()).c_str(), sizeof(record.LastName) - 1);
                    record.LastName[sizeof(record.LastName) - 1] = '\0';
                    strncpy(record.StartDate, utility::conversions::to_utf8string(request_data[U("StartDate")].as_string()).c_str(), sizeof(record.StartDate) - 1);
                    record.StartDate[sizeof(record.StartDate) - 1] = '\0';
                    strncpy(record.ExitDate, utility::conversions::to_utf8string(request_data[U("ExitDate")].as_string()).c_str(), sizeof(record.ExitDate) - 1);
                    record.ExitDate[sizeof(record.ExitDate) - 1] = '\0';
                    strncpy(record.Title, utility::conversions::to_utf8string(request_data[U("Title")].as_string()).c_str(), sizeof(record.Title) - 1);
                    record.Title[sizeof(record.Title) - 1] = '\0';
                    strncpy(record.Supervisor, utility::conversions::to_utf8string(request_data[U("Supervisor")].as_string()).c_str(), sizeof(record.Supervisor) - 1);
                    record.Supervisor[sizeof(record.Supervisor) - 1] = '\0';
                    strncpy(record.ADEmail, utility::conversions::to_utf8string(request_data[U("ADEmail")].as_string()).c_str(), sizeof(record.ADEmail) - 1);
                    record.ADEmail[sizeof(record.ADEmail) - 1] = '\0';
                    strncpy(record.BusinessUnit, utility::conversions::to_utf8string(request_data[U("BusinessUnit")].as_string()).c_str(), sizeof(record.BusinessUnit) - 1);
                    record.BusinessUnit[sizeof(record.BusinessUnit) - 1] = '\0';
                    strncpy(record.EmployeeStatus, utility::conversions::to_utf8string(request_data[U("EmployeeStatus")].as_string()).c_str(), sizeof(record.EmployeeStatus) - 1);
                    record.EmployeeStatus[sizeof(record.EmployeeStatus) - 1] = '\0';
                    strncpy(record.EmployeeType, utility::conversions::to_utf8string(request_data[U("EmployeeType")].as_string()).c_str(), sizeof(record.EmployeeType) - 1);
                    record.EmployeeType[sizeof(record.EmployeeType) - 1] = '\0';
                    strncpy(record.PayZone, utility::conversions::to_utf8string(request_data[U("PayZone")].as_string()).c_str(), sizeof(record.PayZone) - 1);
                    record.PayZone[sizeof(record.PayZone) - 1] = '\0';
                    strncpy(record.EmployeeClassificationType, utility::conversions::to_utf8string(request_data[U("EmployeeClassificationType")].as_string()).c_str(), sizeof(record.EmployeeClassificationType) - 1);
                    record.EmployeeClassificationType[sizeof(record.EmployeeClassificationType) - 1] = '\0';
                    strncpy(record.TerminationType, utility::conversions::to_utf8string(request_data[U("TerminationType")].as_string()).c_str(), sizeof(record.TerminationType) - 1);
                    record.TerminationType[sizeof(record.TerminationType) - 1] = '\0';
                    strncpy(record.TerminationDescription, utility::conversions::to_utf8string(request_data[U("TerminationDescription")].as_string()).c_str(), sizeof(record.TerminationDescription) - 1);
                    record.TerminationDescription[sizeof(record.TerminationDescription) - 1] = '\0';
                    strncpy(record.DepartmentType, utility::conversions::to_utf8string(request_data[U("DepartmentType")].as_string()).c_str(), sizeof(record.DepartmentType) - 1);
                    record.DepartmentType[sizeof(record.DepartmentType) - 1] = '\0';
                    strncpy(record.Division, utility::conversions::to_utf8string(request_data[U("Division")].as_string()).c_str(), sizeof(record.Division) - 1);
                    record.Division[sizeof(record.Division) - 1] = '\0';
                    strncpy(record.DOB, utility::conversions::to_utf8string(request_data[U("DOB")].as_string()).c_str(), sizeof(record.DOB) - 1);
                    record.DOB[sizeof(record.DOB) - 1] = '\0';
                    strncpy(record.State, utility::conversions::to_utf8string(request_data[U("State")].as_string()).c_str(), sizeof(record.State) - 1);
                    record.State[sizeof(record.State) - 1] = '\0';
                    strncpy(record.JobFunctionDescription, utility::conversions::to_utf8string(request_data[U("JobFunctionDescription")].as_string()).c_str(), sizeof(record.JobFunctionDescription) - 1);
                    record.JobFunctionDescription[sizeof(record.JobFunctionDescription) - 1] = '\0';
                    strncpy(record.GenderCode, utility::conversions::to_utf8string(request_data[U("GenderCode")].as_string()).c_str(), sizeof(record.GenderCode) - 1);
                    record.GenderCode[sizeof(record.GenderCode) - 1] = '\0';
                    strncpy(record.LocationCode, utility::conversions::to_utf8string(request_data[U("LocationCode")].as_string()).c_str(), sizeof(record.LocationCode) - 1);
                    record.LocationCode[sizeof(record.LocationCode) - 1] = '\0';
                    strncpy(record.RaceDesc, utility::conversions::to_utf8string(request_data[U("RaceDesc")].as_string()).c_str(), sizeof(record.RaceDesc) - 1);
                    record.RaceDesc[sizeof(record.RaceDesc) - 1] = '\0';
                    strncpy(record.MaritalDesc, utility::conversions::to_utf8string(request_data[U("MaritalDesc")].as_string()).c_str(), sizeof(record.MaritalDesc) - 1);
                    record.MaritalDesc[sizeof(record.MaritalDesc) - 1] = '\0';
                    strncpy(record.PerformanceScore, utility::conversions::to_utf8string(request_data[U("PerformanceScore")].as_string()).c_str(), sizeof(record.PerformanceScore) - 1);
                    record.PerformanceScore[sizeof(record.PerformanceScore) - 1] = '\0';
                    record.CurrentEmployeeRating = request_data[U("CurrentEmployeeRating")].as_double();

                    AVLFile<int, EmployeeRecordAVL> avlFile(table_name+"_avl.dat");
                    int reads = 0;
                    int writes = 0;
                    avlFile.insert(record, reads, writes);
                    json::value responseJson;
                    responseJson[U("reads")] = json::value::number(reads);
                    responseJson[U("writes")] = json::value::number(writes);
                    request.reply(status_codes::OK, responseJson);
                } else {
                    request.reply(status_codes::BadRequest, U("Invalid record format."));
                }
              }).wait();
            }
            if(table_name == "game"){
                request.extract_json().then([=](json::value request_data) {
                if (request_data.has_field(U("key")) && 
                    request_data.has_field(U("Rank")) && 
                    request_data.has_field(U("GameTitle")) && 
                    request_data.has_field(U("Platform")) && 
                    request_data.has_field(U("Year")) && 
                    request_data.has_field(U("Genre")) && 
                    request_data.has_field(U("Publisher")) && 
                    request_data.has_field(U("NorthAmerica")) && 
                    request_data.has_field(U("Europe")) && 
                    request_data.has_field(U("Japan")) && 
                    request_data.has_field(U("RestOfWorld")) && 
                    request_data.has_field(U("Global")) && 
                    request_data.has_field(U("Review"))) {

                    GameRecordAVL record;
                    record.key = request_data[U("key")].as_integer();
                    record.Rank = request_data[U("Rank")].as_integer();
                    strncpy(record.GameTitle, utility::conversions::to_utf8string(request_data[U("GameTitle")].as_string()).c_str(), sizeof(record.GameTitle) - 1);
                    record.GameTitle[sizeof(record.GameTitle) - 1] = '\0';
                    strncpy(record.Platform, utility::conversions::to_utf8string(request_data[U("Platform")].as_string()).c_str(), sizeof(record.Platform) - 1); 
                    record.Platform[sizeof(record.Platform) - 1] = '\0';
                    record.Year = request_data[U("Year")].as_integer();
                    strncpy(record.Genre, utility::conversions::to_utf8string(request_data[U("Genre")].as_string()).c_str(), sizeof(record.Genre) - 1); 
                    record.Genre[sizeof(record.Genre) - 1] = '\0';
                    strncpy(record.Publisher, utility::conversions::to_utf8string(request_data[U("Publisher")].as_string()).c_str(), sizeof(record.Publisher) - 1); 
                    record.Publisher[sizeof(record.Publisher) - 1] = '\0';
                    record.NorthAmerica = request_data[U("NorthAmerica")].as_double();
                    record.Europe = request_data[U("Europe")].as_double();
                    record.Japan = request_data[U("Japan")].as_double();
                    record.RestOfWorld = request_data[U("RestOfWorld")].as_double();
                    record.Global = request_data[U("Global")].as_double();
                    record.Review = request_data[U("Review")].as_double();

                    int reads = 0;
                    int writes = 0;
                    AVLFile<int, GameRecordAVL> avlFile(table_name+"_avl.dat");
                    avlFile.insert(record, reads, writes);

                    json::value responseJson;
                    responseJson[U("reads")] = json::value::number(reads);
                    responseJson[U("writes")] = json::value::number(writes);
                    request.reply(status_codes::OK, responseJson);
                } else {
                    request.reply(status_codes::BadRequest, U("Invalid record format."));
                }
              }).wait();
            }
        } else {
            request.reply(status_codes::BadRequest, U("Filename parameter is required."));
        }
    }

    void post_csv(http_request request, const uri& uri) {
        auto query = uri::split_query(uri.query());
        if (query.find(U("table_name")) != query.end()) {
            std::string table_name = utility::conversions::to_utf8string(query[U("table_name")]);

            request.extract_json().then([=](json::value request_data) {
                if (request_data.has_field(U("csv_path"))) {
                    std::string csv_path = utility::conversions::to_utf8string(request_data[U("csv_path")].as_string());

                    try {
                        if (table_name == "employee") {
                            int sum_reads = 0;
                            int sum_writes = 0;
                            AVLFile<int, EmployeeRecordAVL> avlFile(table_name + "_avl.dat");
                            std::ifstream file(csv_path);
                            if (!file.is_open()) {
                                throw std::runtime_error("Error al abrir el archivo CSV.");
                            }

                            std::string line;
                            // Ignorar el encabezado
                            std::getline(file, line);
                            
                            while (std::getline(file, line)) {
                                try {
                                    std::vector<std::string> fields = split(line);
                                    if (fields.size() < 20) { 
                                        std::cerr << "Error: Línea con menos de 20 campos." << std::endl;
                                        continue; 
                                    }

                                    EmployeeRecordAVL record;
                                    record.key = std::stoi(fields[0]);
                                    strcpy(record.FirstName, fields[1].c_str());
                                    strcpy(record.LastName, fields[2].c_str());
                                    strcpy(record.StartDate, fields[3].c_str());
                                    strcpy(record.ExitDate, fields[4].c_str());
                                    strcpy(record.Title, fields[5].c_str());
                                    strcpy(record.Supervisor, fields[6].c_str());
                                    strcpy(record.ADEmail, fields[7].c_str());
                                    strcpy(record.BusinessUnit, fields[8].c_str());
                                    strcpy(record.EmployeeStatus, fields[9].c_str());
                                    strcpy(record.EmployeeType, fields[10].c_str());
                                    strcpy(record.PayZone, fields[11].c_str());
                                    strcpy(record.EmployeeClassificationType, fields[12].c_str());
                                    strcpy(record.TerminationType, fields[13].c_str());
                                    strcpy(record.TerminationDescription, fields[14].c_str());
                                    strcpy(record.DepartmentType, fields[15].c_str());
                                    strcpy(record.Division, fields[16].c_str());
                                    strcpy(record.DOB, fields[17].c_str());
                                    strcpy(record.State, fields[18].c_str());
                                    strcpy(record.JobFunctionDescription, fields[19].c_str());
                                    strcpy(record.GenderCode, fields[20].c_str());
                                    strcpy(record.LocationCode, fields[21].c_str());
                                    strcpy(record.RaceDesc, fields[22].c_str());
                                    strcpy(record.MaritalDesc, fields[23].c_str());
                                    strcpy(record.PerformanceScore, fields[24].c_str());
                                    record.CurrentEmployeeRating = std::stod(fields[25]);

                                    int reads = 0;
                                    int writes = 0;
                                    avlFile.insert(record, reads, writes);
                                    sum_reads += reads; 
                                    sum_writes += writes;
                                } catch (const std::exception &e) {
                                    std::cerr << "Error al procesar la línea: " << line << " - " << e.what() << std::endl;
                                    continue; // Continuar con la siguiente línea
                                }
                            }
                            cout << "End Reading" << endl;
                            file.close();
                            json::value responseJson;
                            responseJson[U("message")] = json::value::string(U("CSV loaded."));
                            responseJson[U("reads")] = json::value::number(sum_reads);
                            responseJson[U("writes")] = json::value::number(sum_writes);
                            request.reply(status_codes::OK, responseJson);
                        } else if (table_name == "game") {
                            int sum_reads = 0;
                            int sum_writes = 0;
                            AVLFile<int, GameRecordAVL> avlFile(table_name + "_avl.dat");
                            std::ifstream file(csv_path);
                            if (!file.is_open()) {
                                throw std::runtime_error("Error al abrir el archivo CSV.");
                            }

                            std::string line;
                            std::getline(file, line); 
                            
                            while (std::getline(file, line)) {
                                try {
                                    std::vector<std::string> fields = split(line);
                                    if (fields.size() < 13) {
                                        std::cerr << "Error: Línea con menos de 13 campos." << std::endl;
                                        continue; 
                                    }

                                    GameRecordAVL record;
                                    record.key = std::stoi(fields[0]);
                                    record.Rank = std::stoi(fields[1]);
                                    strcpy(record.GameTitle, fields[2].c_str());
                                    strcpy(record.Platform, fields[3].c_str());
                                    record.Year = static_cast<int>(std::stod(fields[4]));
                                    strcpy(record.Genre, fields[5].c_str());
                                    strcpy(record.Publisher, fields[6].c_str());
                                    record.NorthAmerica = std::stod(fields[7]);
                                    record.Europe = std::stod(fields[8]);
                                    record.Japan = std::stod(fields[9]);
                                    record.RestOfWorld = std::stod(fields[10]);
                                    record.Global = std::stod(fields[11]);
                                    record.Review = std::stod(fields[12]);

                                    int reads = 0;
                                    int writes = 0;
                                    avlFile.insert(record, reads, writes);
                                    sum_reads += reads; 
                                    sum_writes += writes;
                                } catch (const std::exception &e) {
                                    std::cerr << "Error al procesar la línea: " << line << " - " << e.what() << std::endl;
                                    continue; // Continuar con la siguiente línea
                                }
                            }
                            file.close();
                            json::value responseJson;
                            responseJson[U("message")] = json::value::string(U("CSV loaded."));
                            responseJson[U("reads")] = json::value::number(sum_reads);
                            responseJson[U("writes")] = json::value::number(sum_writes);
                            request.reply(status_codes::OK, responseJson);
                        } else {
                            request.reply(status_codes::BadRequest, U("Invalid table name."));
                        }
                    } catch (const std::exception &e) {
                        request.reply(status_codes::BadRequest, utility::conversions::to_string_t(e.what()));
                    }
                } else {
                    request.reply(status_codes::BadRequest, U("Missing csv_path."));
                }
            }).wait(); // Espera a que la tarea asíncrona se complete
        } else {
            request.reply(status_codes::BadRequest, U("Missing table_name."));
        }
    }


    void delete_record(http_request request, const uri& uri) {
        auto query = uri::split_query(uri.query());
        if (query.find(U("key")) != query.end() && query.find(U("table_name")) != query.end()) {
            int key = std::stoi(utility::conversions::to_utf8string(query[U("key")]));
            std::string table_name = utility::conversions::to_utf8string(query[U("table_name")]);

            if(table_name == "employee"){
                AVLFile<int, EmployeeRecordAVL> avlFile(table_name+"_avl.dat");
                int reads = 0;
                int writes = 0;
                avlFile.delete_by_key(key, reads, writes);
                json::value responseJson;
                responseJson[U("message")] = json::value::string(U("Record deleted successfully."));
                responseJson[U("reads")] = json::value::number(reads);
                responseJson[U("writes")] = json::value::number(writes);
                request.reply(status_codes::OK, responseJson);
            } if (table_name == "game"){
                AVLFile<int, GameRecordAVL> avlFile(table_name+"_avl.dat");
                int reads = 0;
                int writes = 0;
                avlFile.delete_by_key(key, reads, writes);
                json::value responseJson;
                responseJson[U("message")] = json::value::string(U("Record deleted successfully."));
                responseJson[U("reads")] = json::value::number(reads);
                responseJson[U("writes")] = json::value::number(writes);
                request.reply(status_codes::OK, responseJson);
            }
        } else {
            request.reply(status_codes::BadRequest, U("Key and filename parameters are required."));
        }
    }
};