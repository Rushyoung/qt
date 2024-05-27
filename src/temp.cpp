//
// Created by 小灰灰 on 24-5-24.
//
#include <graphics.h>
#include <cmath>
#include <graphics.h>
#include <easyx.h>
#include <random>
#include <thread>
//#include "basetype.hpp"
int random(int min, int max) {
    std::random_device rd;
    std::default_random_engine gen(rd());
    std::uniform_int_distribution<int> dis(min, max);
    return dis(gen);
}

//double distance(const position& a, const position& b) {
//    return sqrt((a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y));
//}
