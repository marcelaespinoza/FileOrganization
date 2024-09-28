#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <cpprest/uri.h>
#include "avl.h"

using namespace web;
using namespace http;

class AVLHandler {
public:
    void get_all(http_request request, const uri& uri) {
        auto query = uri::split_query(uri.query());
        if (query.find(U("filename")) != query.end()) {
            std::string filename = utility::conversions::to_utf8string(query[U("filename")]);

            AVLFile avlFile(filename);
            auto inorderResult = avlFile.inorder();

            json::value response_data = json::value::array();
            for (size_t i = 0; i < inorderResult.size(); ++i) {
                json::value recordJson;
                recordJson[U("cod")] = json::value::number(inorderResult[i].cod);
                recordJson[U("nombre")] = json::value::string(utility::conversions::to_string_t(inorderResult[i].nombre));
                recordJson[U("ciclo")] = json::value::number(inorderResult[i].ciclo);
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

            AVLFile avlFile(filename);
            Record record = avlFile.search_by_key(key);

            if (record.cod != 0) {
                json::value recordJson;
                recordJson[U("cod")] = json::value::number(record.cod);
                recordJson[U("nombre")] = json::value::string(utility::conversions::to_string_t(record.nombre));
                recordJson[U("ciclo")] = json::value::number(record.ciclo);

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

            AVLFile avlFile(filename);
            auto rangeResult = avlFile.rangeSearch(start, end);

            json::value response_data = json::value::array();
            for (size_t i = 0; i < rangeResult.size(); ++i) {
                json::value recordJson;
                recordJson[U("cod")] = json::value::number(rangeResult[i].cod);
                recordJson[U("nombre")] = json::value::string(utility::conversions::to_string_t(rangeResult[i].nombre));
                recordJson[U("ciclo")] = json::value::number(rangeResult[i].ciclo);
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
                if (request_data.has_field(U("cod")) && request_data.has_field(U("nombre")) && request_data.has_field(U("ciclo"))) {
                    Record record(
                        request_data[U("cod")].as_integer(),
                        utility::conversions::to_utf8string(request_data[U("nombre")].as_string()).c_str(),
                        request_data[U("ciclo")].as_integer()
                    );

                    AVLFile avlFile(filename);
                    avlFile.insert(record);
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

            AVLFile avlFile(filename);
            avlFile.delete_by_key(key);
            request.reply(status_codes::OK, U("Record deleted successfully."));
        } else {
            request.reply(status_codes::BadRequest, U("Key and filename parameters are required."));
        }
    }
};

// No olvides registrar las rutas correspondientes en el servidor principal para utilizar este manejador.

