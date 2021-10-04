//
// Created by 12784 on 2021/9/30.
//

#include "UtilityClasses.h"

InTime::InTime() : start(std::chrono::high_resolution_clock::now()), isFinished(false) {}

InTime::~InTime()
{
    Stop();
}

void InTime::Stop()
{
    if (isFinished) return;
    auto end = std::chrono::high_resolution_clock::now();
    //转换成毫秒，long long int类型
    auto _startValue = std::chrono::time_point_cast<std::chrono::milliseconds>(start).time_since_epoch();
    auto _endValue = std::chrono::time_point_cast<std::chrono::milliseconds>(end).time_since_epoch();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(_endValue - _startValue).count() / 1000.0;
    std::cout << "耗时" << duration << "s" << std::endl;
    isFinished = true;
}

void InTime::ReStart()
{
    start = std::chrono::high_resolution_clock::now();
    isFinished = false;
}
