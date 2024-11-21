#include <iostream>
#include "headers/Peer.h"
#include "asio.hpp"


using asio::ip::tcp;

void start_server(unsigned short port) {
    try {
        asio::io_context io_context;
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), port));

        std::cout << "Server listening on port " << port << "..." << std::endl;

        tcp::socket socket(io_context);
        acceptor.accept(socket);

        std::cout << "Client connected!" << std::endl;

        std::string message = "Hello from server!";
        asio::write(socket, asio::buffer(message));

        char data[1024];
        size_t length = socket.read_some(asio::buffer(data));
        std::cout << "Received from client: " << std::string(data, length) << std::endl;
    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}



int main(){


    start_server(5000);

    return 0;
}