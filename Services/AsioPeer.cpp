#include "asio.hpp"

#include "thread"
#include <iostream>


using asio::ip::tcp;

class Peer {

public :
    Peer(asio::io_context& io_context , int port ): io_context_(io_context), acceptor_(io_context,tcp::endpoint(tcp::v4(),port))
    ,socket_(io_context){

        start_read();

    }

    void connect_to_peer(const std::string& host, int port ) {


    }

private:
    void start_read() {

        asio::async_read_until(socket_, asio::dynamic_buffer(buffer_),'\n',[this](std::error))

    }

    asio::io_context& io_context_;
    tcp::acceptor acceptor_;
    tcp::socket socket_;
    std::string buffer_;

};
