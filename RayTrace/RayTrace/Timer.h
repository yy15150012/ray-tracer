//
// Created by Administrator on 2022/8/22.
//

#ifndef RAY_TRACER_TIMER_H
#define RAY_TRACER_TIMER_H
#include <ctime>
#include "iostream"
#include "stdlib.h"
#include "string"
#include "regex"

using namespace std;
static string getTime() {
    time_t now = time(nullptr);
    tm* local_time = localtime(&now);
    string res = asctime(local_time);
    std::regex newlines_reg("\n+");
    return std::regex_replace(res, newlines_reg, "");;
}


#endif //RAY_TRACER_TIMER_H
