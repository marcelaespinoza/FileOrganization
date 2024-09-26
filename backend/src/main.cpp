#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <cpprest/uri.h>
#include "avl.h"
#include "logger.h"

using namespace web;
using namespace http;
using namespace http::experimental::listener;

Logger logger("server.log");

class Server_BD {
public:
    Server_BD() : listener("http://localhost:8080") {
        listener.support(methods::GET, std::bind(&Server_BD::handle_get, this, std::placeholders::_1));
        listener.support(methods::POST, std::bind(&Server_BD::handle_post, this, std::placeholders::_1));
        listener.support(methods::DEL, std::bind(&Server_BD::handle_delete, this, std::placeholders::_1));
    }

    void open() {
        listener
            .open()
            .then([this]() {
                ucout << "Starting to listen at: " << listener.uri().to_string() << std::endl;
            })
            .wait();
    }

private:
    http_listener listener;

    void handle_get(http_request request) {
        logger.log("Received GET request.");
        auto query = uri::split_query(request.relative_uri().query());

        // Respuesta para Hello World
        if (request.request_uri().path() == U("/")) {
            logger.log("Hello World request received.");
            json::value response_data;
            response_data[U("message")] = json::value::string(U("Hello, World!"));
            request.reply(status_codes::OK, response_data);
            return;
        }

        if (query.find(U("index")) != query.end() && query[U("index")] == U("avl")) {
            if (request.request_uri().path() == U("/get_all")) {
                // Lógica para obtener todos los registros del AVL
                std::string filename;
                if (query.find(U("filename")) != query.end()) {
                    filename = utility::conversions::to_utf8string(query[U("filename")]);
                    logger.log("Received filename: " + filename);

                    AVLFile avlFile(filename);
                    auto inorderResult = avlFile.inorder(); // Suponiendo que esta función devuelve vector<Record>

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
                    logger.log("Filename parameter is missing.");
                    request.reply(status_codes::BadRequest, U("Filename parameter is required."));
                }
            } else if (request.request_uri().path() == U("/get_record")) {
                // Lógica para obtener un registro específico por key (cod)
                if (query.find(U("key")) != query.end()) {
                    int key = std::stoi(utility::conversions::to_utf8string(query[U("key")]));
                    std::string filename;

                    if (query.find(U("filename")) != query.end()) {
                        filename = utility::conversions::to_utf8string(query[U("filename")]);
                        logger.log("Received filename: " + filename);

                        AVLFile avlFile(filename);
                        Record record = avlFile.search_by_key(key);

                        if (record.cod != 0) { // Asumiendo que cod = 0 indica que no existe el registro
                            json::value recordJson;
                            recordJson[U("cod")] = json::value::number(record.cod);
                            recordJson[U("nombre")] = json::value::string(utility::conversions::to_string_t(record.nombre));
                            recordJson[U("ciclo")] = json::value::number(record.ciclo);

                            request.reply(status_codes::OK, recordJson);
                            logger.log("Record found with key: " + std::to_string(key));
                        } else {
                            logger.log("Record not found for key: " + std::to_string(key));
                            request.reply(status_codes::NotFound, U("Record not found."));
                        }
                    } else {
                        logger.log("Filename parameter is missing.");
                        request.reply(status_codes::BadRequest, U("Filename parameter is required."));
                    }
                } else {
                    logger.log("Key parameter is missing.");
                    request.reply(status_codes::BadRequest, U("Key parameter is required."));
                }
            } else if (request.request_uri().path() == U("/get_between")) {
                // Lógica para obtener los registros en un rango de claves (start - end)
                if (query.find(U("start")) != query.end() && query.find(U("end")) != query.end()) {
                    int start = std::stoi(utility::conversions::to_utf8string(query[U("start")]));
                    int end = std::stoi(utility::conversions::to_utf8string(query[U("end")]));
                    std::string filename;

                    if (query.find(U("filename")) != query.end()) {
                        filename = utility::conversions::to_utf8string(query[U("filename")]);
                        logger.log("Received filename: " + filename);

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
                        logger.log("Records found between " + std::to_string(start) + " and " + std::to_string(end));
                    } else {
                        logger.log("Filename parameter is missing.");
                        request.reply(status_codes::BadRequest, U("Filename parameter is required."));
                    }
                } else {
                    logger.log("Start or end parameter is missing.");
                    request.reply(status_codes::BadRequest, U("Start and end parameters are required."));
                }
            } else {
                logger.log("Path not found.");
                request.reply(status_codes::NotFound, U("404 Not Found: Invalid path."));
            }
        } else {
            logger.log("Invalid index parameter or index not found.");
            request.reply(status_codes::NotFound, U("404 Not Found: Invalid index parameter."));
        }
    }

    void handle_post(http_request request) {
        logger.log("Received POST request.");
        auto query = uri::split_query(request.relative_uri().query());

        if (query.find(U("index")) != query.end() && query[U("index")] == U("avl")) {
            std::string filename;

            if (query.find(U("filename")) != query.end()) {
                filename = utility::conversions::to_utf8string(query[U("filename")]);
                logger.log("Received filename: " + filename);

                request.extract_json().then([=](json::value request_data) {
                    if (request_data.has_field(U("cod")) && request_data.has_field(U("nombre")) && request_data.has_field(U("ciclo"))) {
                        Record record(
                            request_data[U("cod")].as_integer(),
                            utility::conversions::to_utf8string(request_data[U("nombre")].as_string()).c_str(),
                            request_data[U("ciclo")].as_integer()
                        );

                        AVLFile avlFile(filename);
                        avlFile.insert(record); // Llama a la función para insertar el registro
                        logger.log("Record inserted: " + std::to_string(record.cod));
                        request.reply(status_codes::OK, U("Record added successfully."));
                    } else {
                        logger.log("Invalid record format.");
                        request.reply(status_codes::BadRequest, U("Invalid record format."));
                    }
                }).wait();
            } else {
                logger.log("Filename parameter is missing.");
                request.reply(status_codes::BadRequest, U("Filename parameter is required."));
            }
        } else {
            logger.log("Invalid index parameter or index not found.");
            request.reply(status_codes::NotFound, U("404 Not Found: Invalid index parameter."));
        }
    }

    void handle_delete(http_request request) {
        logger.log("Received DELETE request.");
        auto query = uri::split_query(request.relative_uri().query());

        if (query.find(U("index")) != query.end() && query[U("index")] == U("avl")) {
            if (query.find(U("key")) != query.end()) {
                int key = std::stoi(utility::conversions::to_utf8string(query[U("key")]));
                std::string filename;

                if (query.find(U("filename")) != query.end()) {
                    filename = utility::conversions::to_utf8string(query[U("filename")]);
                    logger.log("Received filename: " + filename);

                    AVLFile avlFile(filename);
                    avlFile.delete_by_key(key); // Llama a la función para eliminar el registro
                    logger.log("Record deleted with key: " + std::to_string(key));
                    request.reply(status_codes::OK, U("Record deleted successfully."));
                } else {
                    logger.log("Filename parameter is missing.");
                    request.reply(status_codes::BadRequest, U("Filename parameter is required."));
                }
            } else {
                logger.log("Key parameter is missing.");
                request.reply(status_codes::BadRequest, U("Key parameter is required."));
            }
        } else {
            logger.log("Invalid index parameter or index not found.");
            request.reply(status_codes::NotFound, U("404 Not Found: Invalid index parameter."));
        }
    }
};

int main() {
    Server_BD server;
    server.open();
    std::cout << "Server running..." << std::endl;
    std::string input;
    std::getline(std::cin, input); // Mantener el servidor ejecutándose
    return 0;
}

