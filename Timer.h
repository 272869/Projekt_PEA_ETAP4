//
// Created by pitko on 19.01.2025.
//

#ifndef PROJEKT_4_TIMER_H
#define PROJEKT_4_TIMER_H
#include <cstdint>
#include <chrono>

class Timer {
    std::chrono::high_resolution_clock::time_point t0;
    std::chrono::high_resolution_clock::time_point t1;

public:
    void start();
    void stop();
    uint64_t timeperiod();
};


#endif //PROJEKT_4_TIMER_H
