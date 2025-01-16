#pragma once

#include <chrono>

class Timer {
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime, endTime;
    double elapsedTime; // Time in milliseconds

public:
    Timer();
    void start();          // Start the timer
    void stop();           // Stop the timer
    double getElapsed();   // Get elapsed time in milliseconds
    void printElapsed(const char* label); // Print elapsed time with a C-style string label
};
