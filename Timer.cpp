//
// Created by pitko on 19.01.2025.
//

#include <cstdint>
#include <chrono>
#include "Timer.h"

void Timer::start() {
    t0 = std::chrono::high_resolution_clock::now();
}

void Timer::stop() {
    t1 = std::chrono::high_resolution_clock::now();
}

uint64_t Timer::timeperiod() {
    std::chrono::duration<double> finalTime = t1 - t0;
    return static_cast<uint64_t>(finalTime.count() * 1000000);
}

uint64_t Timer::subTime() {
    std::chrono::high_resolution_clock::time_point temp = std::chrono::high_resolution_clock::now();
    return static_cast<uint64_t>((temp - t0).count() * 1000000);
}