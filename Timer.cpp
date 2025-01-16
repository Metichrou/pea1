#include "Timer.h"
#include <iostream>

Timer::Timer() : elapsedTime(0.0) {}

void Timer::start() {
    startTime = std::chrono::high_resolution_clock::now();
}

void Timer::stop() {
    endTime = std::chrono::high_resolution_clock::now();
    elapsedTime = std::chrono::duration<double, std::milli>(endTime - startTime).count();
}

double Timer::getElapsed() {
    return elapsedTime;
}

void Timer::printElapsed(const char* label) {
    std::cout << label << " execution time: " << elapsedTime << " ms" << std::endl;
}
