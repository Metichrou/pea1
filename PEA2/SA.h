#pragma once
#include <cmath> // For log and exp

class SA {
private:
    int** costMatrix;    // Cost matrix (graph)
    int size;            // Number of cities
    double initialTemp;  // Initial temperature
    double coolingRate;  // Cooling rate
    int maxIterations;   // Maximum iterations
    int maxTimeSA;       // Maximum time in seconds
    int coolingChoice;   // Cooling method choice (1 = Logarithmic, 2 = Linear, 3 = Geometric)

    // Generates a random neighbor solution
    void generateNeighbor(int* currentSolution, int* neighbor);

    // Calculates the cost of a solution
    int calculateCost(const int* solution);

    // Cooling functions
    double logarithmicCooling(double temperature, int iteration);
    double linearCooling(double temperature, int iteration);
    double geometricCooling(double temperature);

public:
    // Constructor
    SA(int** costMatrix, int size, double initialTemp, double coolingRate, int maxIterations, int maxTimeSA, int coolingChoice);

    // Main Simulated Annealing function
    void simulatedAnnealing(int* bestSolution, int* bestCost);

    // Destructor
    ~SA();
};
