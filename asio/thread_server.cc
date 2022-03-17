#include <asio.hpp>
#include <memory>
#include <iostream>
using namespace asio;

typedef std::shared_ptr<ip::tcp::socket> socket_ptr;

void client_handler(socket_ptr sock) {
    while (true) {
        char buff[65536];
        size_t len = sock->read_some(buffer(buff));
       //if (len > 0) write(*sock, buffer(buff));
        std::cout << buff << std::endl;
    }
}

int main() {
    io_service service_io;
    std::vector<std::thread> threads;
    ip::tcp::endpoint end_point(ip::tcp::v4(), 1989);
    ip::tcp::acceptor acc(service_io, end_point);

    while (true)
    {
        socket_ptr client_socket_ptr(new ip::tcp::socket(service_io));
        acc.accept(*client_socket_ptr);
        threads.emplace_back(client_handler, client_socket_ptr);
    }

    for (auto& i : threads) i.join();
}
