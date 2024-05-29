//
// Created by 小小喵姬 on 24-5-17.
//
#include "../include/net.hpp"
void udp_server::run() {
    try {
        asio::io_context io_context;

        asio::ip::udp::socket socket(io_context, asio::ip::udp::endpoint(asio::ip::udp::v4(), _port));
        int flag = 0;
        for (;;) {
            char data[1024];
            std::memset(data, 0, sizeof(data));
            asio::ip::udp::endpoint sender_endpoint;
            size_t length = socket.receive_from(asio::buffer(data), sender_endpoint);

            std::cout << "Received message: " << data << " from " << sender_endpoint << std::endl;
            flag++;
            std::cout << "flag:" << flag << std::endl;
            socket.send_to(asio::buffer(data, length), sender_endpoint);
        }
    }
    catch (std::exception& e) {
//        std::cerr << e.what() << std::endl;
    }
}

void udp_client::send(std::string _stream) {
    std::string port = std::to_string(_port);
    try {
        asio::io_context io_context;

        asio::ip::udp::resolver resolver(io_context);
        asio::ip::udp::endpoint endpoint = *resolver.resolve(asio::ip::udp::v4(), _host, port).begin();

        asio::ip::udp::socket socket(io_context);
        socket.open(asio::ip::udp::v4());


        socket.send_to(asio::buffer(_stream), endpoint);

        char reply[1024];
        asio::ip::udp::endpoint sender_endpoint;
        size_t length = socket.receive_from(asio::buffer(reply), sender_endpoint);

        std::cout << "Reply is: ";
        std::cout.write(reply, length);  // 只输出接收到的数据
        std::cout << std::endl;

        socket.close();
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

long long int unix_time_stamp(){
    auto now = std::chrono::system_clock::now();
    auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
    auto epoch = now_ms.time_since_epoch();
    auto value = std::chrono::duration_cast<std::chrono::milliseconds>(epoch);
    return value.count();
}

void udp_client::letency(){
    std::string port = std::to_string(_port);
    auto first = unix_time_stamp();
    try {
        asio::io_context io_context;

        asio::ip::udp::resolver resolver(io_context);
        asio::ip::udp::endpoint endpoint = *resolver.resolve(asio::ip::udp::v4(), _host, port).begin();

        asio::ip::udp::socket socket(io_context);
        socket.open(asio::ip::udp::v4());


        socket.send_to(asio::buffer(std::to_string(first)), endpoint);

        char reply[1024];
        asio::ip::udp::endpoint sender_endpoint;
        size_t length = socket.receive_from(asio::buffer(reply), sender_endpoint);
        auto second = unix_time_stamp();
        std::cout << second - first << "ms" << std::endl;

        socket.close();
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}