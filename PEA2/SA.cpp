#include "SA.h"
#include <ctime>
#include <cstdlib> // For rand()
#include <cmath>   // For log() and exp()

using namespace std;

// Constructor
SA::SA(int** costMatrix, int size, double initialTemp, double coolingRate, int maxIterations, int maxTimeSA, int coolingChoice)
    : costMatrix(costMatrix), size(size), initialTemp(initialTemp), coolingRate(coolingRate), 
    maxIterations(maxIterations), maxTimeSA(maxTimeSA), coolingChoice(coolingChoice) {
    srand(time(0)); // Seed for random generation
}

// Destructor
SA::~SA() {
    // No specific cleanup is needed in this implementation.
}

// Helper function to manually swap two integers
void manualSwap(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

// Generates a random neighbor solution by swapping two random cities
void SA::generateNeighbor(int* currentSolution, int* neighbor) {
    int i = rand() % size;
    int j = rand() % size;
    while (i == j) {
        j = rand() % size;
    }

    // Copy the current solution into the neighbor
    for (int k = 0; k < size; ++k) {
        neighbor[k] = currentSolution[k];
    }
    // Swap two random cities manually
    manualSwap(neighbor[i], neighbor[j]);
}

// Calculates the cost of a solution
int SA::calculateCost(const int* solution) {
    int cost = 0;
    for (int i = 0; i < size - 1; ++i) {
        cost += costMatrix[solution[i]][solution[i + 1]];
    }
    cost += costMatrix[solution[size - 1]][solution[0]]; // Return to start
    return cost;
}

// Cooling functions
double SA::logarithmicCooling(double temperature, int iteration) {
    return temperature / (1 + coolingRate * log(1 + iteration));
}

double SA::linearCooling(double temperature, int iteration) {
    return temperature - (coolingRate * iteration);
}

double SA::geometricCooling(double temperature) {
    return temperature * coolingRate;
}

// Main Simulated Annealing function
void SA::simulatedAnnealing(int* bestSolution, int* bestCost) {
    int* currentSolution = new int[size];
    bool* visited = new bool[size](); // Track visited cities

    // Select a random starting city
    int startCity = rand() % size;
    currentSolution[0] = startCity;
    visited[startCity] = true;

    // Greedy initialization from the random starting city
    for (int i = 1; i < size; ++i) {
        int minCost = 1000000;
        int nextCity = -1;
        for (int j = 0; j < size; ++j) {
            if (!visited[j] && costMatrix[currentSolution[i - 1]][j] < minCost) {
                minCost = costMatrix[currentSolution[i - 1]][j];
                nextCity = j;
            }
        }
        currentSolution[i] = nextCity;
        visited[nextCity] = true;
    }

    delete[] visited; // Clean up visited array

    // Copy the initial solution to bestSolution
    for (int i = 0; i < size; ++i) {
        bestSolution[i] = currentSolution[i];
    }
    *bestCost = calculateCost(bestSolution);

    double temperature = initialTemp;

    int* bestSol = new int[size];
    for (int i = 0; i < size; ++i) {
        bestSol[i] = currentSolution[i];
    }

    clock_t start = clock();
    int iteration = 0;

    while (((clock() - start) / CLOCKS_PER_SEC) < maxTimeSA 
           && iteration < maxIterations 
           && temperature > 1e-6) {
        int* neighbor = new int[size];
        generateNeighbor(currentSolution, neighbor);

        int neighborCost = calculateCost(neighbor);

        if (neighborCost < *bestCost) {
            *bestCost = neighborCost;
            for (int i = 0; i < size; ++i) {
                bestSol[i] = neighbor[i];
            }
        } else {
            double acceptanceProb = exp((calculateCost(currentSolution) - neighborCost) / temperature);
            if (rand() / (RAND_MAX + 1.0) < acceptanceProb) {
                for (int i = 0; i < size; ++i) {
                    currentSolution[i] = neighbor[i];
                }
            }
        }

        // Update the temperature based on the cooling schedule
        switch (coolingChoice) {
            case 1: temperature = logarithmicCooling(temperature, iteration); break;
            case 2: temperature = linearCooling(temperature, iteration); break;
            case 3: temperature = geometricCooling(temperature); break;
        }

        ++iteration;
        delete[] neighbor;
    }

    for (int i = 0; i < size; ++i) {
        bestSolution[i] = bestSol[i];
    }

    delete[] currentSolution;
    delete[] bestSol;
}
