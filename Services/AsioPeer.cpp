#include "asio.hpp"

#include "thread"
#include <iostream>


using asio::ip::tcp;

class Peer {

public :
    Peer(asio::io_context& io_context , int port ): io_context_(io_context), acceptor_(io_context,tcp::endpoint(tcp::v4(),port))
    ,socket_(io_context){

        start_accept();
    }

    void connect_to_peer(const std::string& host, int port ) {
        tcp::resolver resolver(io_context_);
        auto endpoints = resolver.resolve(host, std::to_string(port));
        asio::async_connect(socket_, endpoints,

            [this](std::error_code ec , const tcp::endpoint&) {

                if (!ec) {

                    std::cout<<"Connected to peer! "<<std::endl;
                    send_message("Hello from Peer !");
                    start_read();

                }
                else {
                    std::cerr << "Failed to connect: "<< ec.message() <<std::endl;
                }
            }
        );

    }

    void send_message(const std::string& message) {
    asio::async_write(socket_, asio::buffer(message + "\n"),
        [this](std::error_code ec, std::size_t) {
            if (!ec) {
                std::cout<< "Message sent!"<< std::endl;

            }
            else {
                std::cerr <<"Send failed: " << ec.message() <<std::endl;
            }

        }
        );
}



private:
    void start_read() {

        asio::async_read_until(socket_, asio::dynamic_buffer(buffer_), '\n',
            [this](std::error_code ec , std::size_t length) {

                if (!ec) {

                    std::string message(buffer_.substr(0, length - 1));

                    buffer_.erase(0,length);
                    std::cout << "Received: " <<message << std::endl;
                    start_read();

                }
                else {
                   std::cerr<< "Read failed: "<< ec.message() << std:: endl;
                }
            });
    }

    void start_accept(){

        acceptor_.async_accept(
            [this] (std:: error_code ec, tcp::socket new_socket) {
                if (!ec) {
                    std::cout << "Accepted a connection!" <<std::endl;
                    socket_ = std::move(new_socket);
                    start_read();
                }
                else {
                    std::cerr <<"Accept failed: " << ec.message() <<std::endl;
                }
                start_accept();
            }
        );

    }

    asio::io_context& io_context_;
    tcp::acceptor acceptor_;
    tcp::socket socket_;
    std::string buffer_;

};


int main() {

    try {
        asio::io_context io_context;

        int port;
        std::cout<< "Start server on: "<<std::endl;
        std::cin>> port;

        int port_to_connect;
        std::cout<< "Connect to port: "<<std::endl;
        std::cin>> port_to_connect;

        Peer peer(io_context,port );

        std::thread([&io_context, &peer, port_to_connect]() {
            std::this_thread::sleep_for(std::chrono::seconds(2));

            std::string wait_cmd;
            std::cin>>wait_cmd;
            peer.connect_to_peer("127.0.0.1",port_to_connect);

            while (true) {

                std::cin>>wait_cmd;
                peer.send_message(wait_cmd);

            }
        }).detach();

        io_context.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " <<e.what() <<std::endl;
    }

    return 0;




}
