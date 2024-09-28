#include <cpprest/http_listener.h>
#include <cpprest/uri.h>
#include "avl_handler.cpp"      // Incluye el manejador AVL
#include "sequential_handler.cpp" // Incluye el manejador secuencial

using namespace web;
using namespace http;
using namespace http::experimental::listener;

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
    AVLHandler avlHandler; // Manejador AVL
    SequentialHandler seqHandler; // Manejador secuencial

    void handle_get(http_request request) {
        auto path = request.request_uri().path();

        if (path.find(U("/avl/get_all")) == 0) {
            avlHandler.get_all(request, request.request_uri());
        } else if (path.find(U("/avl/get_record")) == 0) {
            avlHandler.get_record(request, request.request_uri());
        } else if (path.find(U("/avl/get_between")) == 0) {
            avlHandler.get_between(request, request.request_uri());
        } else if (path.find(U("/sequential/get_all")) == 0) {
            seqHandler.get_all(request, request.request_uri());
        } else if (path.find(U("/sequential/get_record")) == 0) {
            seqHandler.get_record(request, request.request_uri());
        } else if (path.find(U("/sequential/get_between")) == 0) {
            seqHandler.get_between(request, request.request_uri());
        } else {
            request.reply(status_codes::NotFound, U("404 Not Found: Invalid path."));
        }
    }

    void handle_post(http_request request) {
        auto path = request.request_uri().path();

        if (path.find(U("/avl/post_record")) == 0) {
            avlHandler.post_record(request, request.request_uri());
        } else if (path.find(U("/sequential/post_record")) == 0) {
            seqHandler.post_record(request, request.request_uri());
        } else {
            request.reply(status_codes::NotFound, U("404 Not Found: Invalid path."));
        }
    }

    void handle_delete(http_request request) {
        auto path = request.request_uri().path();

        if (path.find(U("/avl/delete_record")) == 0) {
            avlHandler.delete_record(request, request.request_uri());
        } else if (path.find(U("/sequential/delete_record")) == 0) {
            seqHandler.delete_record(request, request.request_uri());
        } else {
            request.reply(status_codes::NotFound, U("404 Not Found: Invalid path."));
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

