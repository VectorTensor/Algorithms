#include "asio.hpp"

#include "thread"
#include <iostream>


using asio::ip::tcp;

class Peer {

public :
    Peer(asio::io_context& io_context , int port ): io_context_(io_context), acceptor_(io_context,tcp::endpoint(tcp::v4(),port))
    ,socket_(io_context){



    }

    void connect_to_peer(const std::string& host, int port ) {




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
