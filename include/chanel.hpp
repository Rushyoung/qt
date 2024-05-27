//
// Created by rushy on 24-5-22.
//

#ifndef TANK_BATTLE_CHANEL_HPP
#define TANK_BATTLE_CHANEL_HPP
#include <string>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <iostream>
#include <map>
#include <queue>
#include "basetype.hpp"


using std::cout;
using std::map;
using std::queue;
using std::string;



template <typename T>
struct chan_message {
    std::mutex lock;
    std::condition_variable cond;
    std::queue<T> messages;
    chan_message() { cout << "chan_message created\n"; }
};

template <typename T>
using channel_map = map<string, std::shared_ptr<chan_message<T>>>;

template <typename T>
class chan {
private:
    static channel_map<T> msg_set;
    std::shared_ptr<chan_message<T>> msg;

public:
    chan(string name) {
        auto it = msg_set.find(name);
        if (it == msg_set.end()) {
            msg = std::make_shared<chan_message<T>>();
            msg_set[name] = msg;
        } else {
            msg = it->second;
        }
    }

    void send(const T& message) {
        std::lock_guard<std::mutex> lock(msg->lock);
        while (msg->messages.size() >= 5) {
            msg->messages.pop();
        }
        msg->messages.push(message);
        msg->cond.notify_one();
    }

    T receive_safe() {
        std::unique_lock<std::mutex> lock(msg->lock);
        msg->cond.wait(lock, [this] { return !msg->messages.empty(); });
        T message = msg->messages.front();
        msg->messages.pop();
        return message;
    }

    T receive() {
        std::lock_guard<std::mutex> lock(msg->lock);
        if (msg->messages.empty()) {
            return T();
        }
        T message = msg->messages.front();
        msg->messages.pop();
        return message;
    }
};

template <typename T>
channel_map<T> chan<T>::msg_set = {};

//int main() {
//    chan("test").send("Hello");
//    cout << chan("test").receive() << "\n";
//    return 0;
//}

#endif //TANK_BATTLE_CHANEL_HPP
