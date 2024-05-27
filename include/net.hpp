//
// Created by 小小喵姬 on 24-5-17.
//
//#define ASIO_STANDALONE
#include <asio.hpp>
#include <iostream>
#include <string>
#include <utility>
#ifndef TANK_BATTLE_NET_HPP
long long int unix_time_stamp();
enum class PacketType : char {
    Hello = 0x01,
    TankInfo,
    Sync
};


class udp_server{
public:
    void run();
    udp_server():_port(0){}
    explicit udp_server(int port):_port(port){}
private:
    int _port;
};

class udp_client{
public:
    udp_client(std::string host, int port):_host(std::move(host)), _port(port){}
    udp_client(udp_client& a){}
    void send(std::string _stream);
    void letency();
private:
    std::string _host;
    int _port;
};

class host{

};


#define TANK_BATTLE_NET_HPP

#endif //TANK_BATTLE_NET_HPP
