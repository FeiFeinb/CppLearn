//
// Created by 12784 on 2021/9/30.
//

#ifndef CPPLEARN_UTILITYCLASSES_H
#define CPPLEARN_UTILITYCLASSES_H

#include "UtilityTemplate.hpp"

class InTime
{
private:
    std::chrono::high_resolution_clock::time_point start;
    bool isFinished;
public:
    InTime();
    ~InTime();
    void Stop();
    void ReStart();
};

#endif //CPPLEARN_UTILITYCLASSES_H
