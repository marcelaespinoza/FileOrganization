#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <cpprest/uri.h>
#include "sequential_file.h"
#include "record_sequential.h"

using namespace web;
using namespace http;

class SequentialHandler {
public:
    void get_all(http_request request, const uri& uri) {
        auto query = uri::split_query(uri.query());
        if (query.find(U("filename")) != query.end()) {
            std::string filename = utility::conversions::to_utf8string(query[U("filename")]);

            SequentialFile<int, Record_S> seqFile(filename, "auxfile.dat");
            auto inorderResult = seqFile.get_all();

            json::value response_data = json::value::array();
            for (size_t i = 0; i < inorderResult.size(); ++i) {
                json::value recordJson;
                recordJson[U("key")] = json::value::number(inorderResult[i].key);
                recordJson[U("data")] = json::value::string(utility::conversions::to_string_t(inorderResult[i].data));
                response_data[i] = recordJson;
            }
            request.reply(status_codes::OK, response_data);
        } else {
            request.reply(status_codes::BadRequest, U("Filename parameter is required."));
        }
    }

    void get_record(http_request request, const uri& uri) {
        auto query = uri::split_query(uri.query());
        if (query.find(U("key")) != query.end() && query.find(U("filename")) != query.end()) {
            int key = std::stoi(utility::conversions::to_utf8string(query[U("key")]));
            std::string filename = utility::conversions::to_utf8string(query[U("filename")]);

            SequentialFile<int, Record_S> seqFile(filename, "auxfile.dat");
            Record_S record = seqFile.search_by_key(key);

            if (record.key != -1) { // Asumiendo que -1 significa "no encontrado"
                json::value recordJson;
                recordJson[U("key")] = json::value::number(record.key);
                recordJson[U("data")] = json::value::string(utility::conversions::to_string_t(record.data));

                request.reply(status_codes::OK, recordJson);
            } else {
                request.reply(status_codes::NotFound, U("Record not found."));
            }
        } else {
            request.reply(status_codes::BadRequest, U("Key and filename parameters are required."));
        }
    }

    void get_between(http_request request, const uri& uri) {
        auto query = uri::split_query(uri.query());
        if (query.find(U("start")) != query.end() && query.find(U("end")) != query.end() && query.find(U("filename")) != query.end()) {
            int start = std::stoi(utility::conversions::to_utf8string(query[U("start")]));
            int end = std::stoi(utility::conversions::to_utf8string(query[U("end")]));
            std::string filename = utility::conversions::to_utf8string(query[U("filename")]);

            SequentialFile<int, Record_S> seqFile(filename, "auxfile.dat");
            std::vector<Record_S> rangeResult = seqFile.get_between(start, end);

            json::value response_data = json::value::array();
            for (size_t i = 0; i < rangeResult.size(); ++i) {
                json::value recordJson;
                recordJson[U("key")] = json::value::number(rangeResult[i].key);
                recordJson[U("data")] = json::value::string(utility::conversions::to_string_t(rangeResult[i].data));
                response_data[i] = recordJson;
            }
            request.reply(status_codes::OK, response_data);
        } else {
            request.reply(status_codes::BadRequest, U("Start, end, and filename parameters are required."));
        }
    }

    void post_record(http_request request, const uri& uri) {
        auto query = uri::split_query(uri.query());
        if (query.find(U("filename")) != query.end()) {
            std::string filename = utility::conversions::to_utf8string(query[U("filename")]);

            request.extract_json().then([=](json::value request_data) {
                if (request_data.has_field(U("key")) && request_data.has_field(U("data"))) {
                    Record_S record(
                        request_data[U("key")].as_integer(),
                        utility::conversions::to_utf8string(request_data[U("data")].as_string()).c_str()
                    );

                    SequentialFile<int, Record_S> seqFile(filename, "auxfile.dat");
                    seqFile.insertIntoAuxFile(record);
                    request.reply(status_codes::OK, U("Record added successfully."));
                } else {
                    request.reply(status_codes::BadRequest, U("Invalid record format."));
                }
            }).wait();
        } else {
            request.reply(status_codes::BadRequest, U("Filename parameter is required."));
        }
    }

    void delete_record(http_request request, const uri& uri) {
        auto query = uri::split_query(uri.query());
        if (query.find(U("key")) != query.end() && query.find(U("filename")) != query.end()) {
            int key = std::stoi(utility::conversions::to_utf8string(query[U("key")]));
            std::string filename = utility::conversions::to_utf8string(query[U("filename")]);

            SequentialFile<int, Record_S> seqFile(filename, "auxfile.dat");
            seqFile.delete_by_key(key);
            request.reply(status_codes::OK, U("Record deleted successfully."));
        } else {
            request.reply(status_codes::BadRequest, U("Key and filename parameters are required."));
        }
    }
};

// No olvides registrar las rutas correspondientes en el servidor principal para utilizar este manejador.

