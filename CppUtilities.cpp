//
// Created by 12784 on 2021/9/30.
//

#include "CppUtilities.h"




InTime::InTime() : start(std::chrono::high_resolution_clock::now()) {}

InTime::~InTime()
{
    Stop();
}

void InTime::Stop()
{
    auto end = std::chrono::high_resolution_clock::now();
    auto _startValue = std::chrono::time_point_cast<std::chrono::microseconds>(start).
            time_since_epoch().count();
    auto _endValue = std::chrono::time_point_cast<std::chrono::microseconds>(end).
            time_since_epoch().count();            //转换成微秒，long long类型
    auto duration = _endValue - _startValue;
    std::cout << duration << "us\n";

}


