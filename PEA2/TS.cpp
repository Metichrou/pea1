#include "TS.h"
#include <ctime>   // For time tracking
#include <cstdlib> // For random number generation

// Constructor
TS::TS(int** costMatrix, int size, int tenure)
    : costMatrix(costMatrix), size(size), tenure(tenure), bestCost(1000000) {
    // Allocate memory for tabu list and best solution
    tabuList = new int*[tenure];
    for (int i = 0; i < tenure; ++i) {
        tabuList[i] = new int[size];
    }
    bestSolution = new int[size];
}

// Destructor
TS::~TS() {
    for (int i = 0; i < tenure; ++i) {
        delete[] tabuList[i];
    }
    delete[] tabuList;
    delete[] bestSolution;
}

// Greedy initialization
// Greedy initialization with a random starting city
void TS::greedyInitialization(int* solution) {
    bool* visited = new bool[size](); // Track visited cities

    // Select a random starting city
    int startCity = rand() % size;
    solution[0] = startCity; // Start from a random city
    visited[startCity] = true;

    // Generate the rest of the path
    for (int i = 1; i < size; ++i) {
        int minCost = 1000000;
        int nextCity = -1;
        for (int j = 0; j < size; ++j) {
            if (!visited[j] && costMatrix[solution[i - 1]][j] < minCost) {
                minCost = costMatrix[solution[i - 1]][j];
                nextCity = j;
            }
        }
        solution[i] = nextCity;
        visited[nextCity] = true;
    }

    delete[] visited;
}


// Calculate cost of a solution
int TS::calculateCost(const int* solution) {
    int totalCost = 0;
    for (int i = 0; i < size - 1; ++i) {
        totalCost += costMatrix[solution[i]][solution[i + 1]];
    }
    totalCost += costMatrix[solution[size - 1]][solution[0]]; // Return to start
    return totalCost;
}

// Check if a solution is tabu
bool TS::isTabu(const int* solution) {
    for (int i = 0; i < tenure; ++i) {
        bool matches = true;
        for (int j = 0; j < size; ++j) {
            if (tabuList[i][j] != solution[j]) {
                matches = false;
                break;
            }
        }
        if (matches) return true;
    }
    return false;
}

// Update tabu list
void TS::updateTabuList(const int* solution) {
    for (int i = 0; i < size; ++i) {
        tabuList[tenure - 1][i] = solution[i];
    }
    for (int i = 0; i < tenure - 1; ++i) {
        for (int j = 0; j < size; ++j) {
            tabuList[i][j] = tabuList[i + 1][j];
        }
    }
}

// Generate neighbors
void TS::generateNeighbor(const int* currentSolution, int* neighbor, int neighborType) {
    for (int i = 0; i < size; ++i) {
        neighbor[i] = currentSolution[i];
    }

    int i = rand() % size;
    int j = rand() % size;
    while (i == j) j = rand() % size;

    if (neighborType == 1) { // Swap
        generateSwapNeighbor(currentSolution, neighbor, i, j);
    } else if (neighborType == 2) { // Insert
        generateInsertNeighbor(currentSolution, neighbor, i, j);
    }
}

void TS::generateSwapNeighbor(const int* solution, int* neighbor, int i, int j) {
    for (int k = 0; k < size; ++k) neighbor[k] = solution[k];
    int temp = neighbor[i];
    neighbor[i] = neighbor[j];
    neighbor[j] = temp;
}

void TS::generateInsertNeighbor(const int* solution, int* neighbor, int i, int j) {
    for (int k = 0; k < size; ++k) neighbor[k] = solution[k];
    int nodeToInsert = neighbor[i];
    if (i < j) {
        for (int k = i; k < j; ++k) neighbor[k] = neighbor[k + 1];
    } else {
        for (int k = i; k > j; --k) neighbor[k] = neighbor[k - 1];
    }
    neighbor[j] = nodeToInsert;
}

// Copy solution
void TS::copySolution(const int* source, int* destination) {
    for (int i = 0; i < size; ++i) {
        destination[i] = source[i];
    }
}

// Tabu Search algorithm
// Tabu Search algorithm with stagnation termination
void TS::tabuSearch(int* optimalSolution, int& optimalCost, int neighborType, double maxTimeTS, int maxIterations) {
    int* currentSolution = new int[size];
    int* bestLocalSolution = new int[size];
    greedyInitialization(currentSolution);
    copySolution(currentSolution, bestSolution);
    bestCost = calculateCost(currentSolution);

    std::time_t startTime = std::time(nullptr);
    int iteration = 0;
    int stagnationCount = 0; // Counter for stagnation

    while (std::difftime(std::time(nullptr), startTime) < maxTimeTS && iteration < maxIterations) {
        int* neighbor = new int[size];
        int bestNeighborCost = 1000000;

        for (int i = 0; i < 100; ++i) { // Generate a fixed number of neighbors
            generateNeighbor(currentSolution, neighbor, neighborType);
            int neighborCost = calculateCost(neighbor);

            if (!isTabu(neighbor) && neighborCost < bestNeighborCost) {
                bestNeighborCost = neighborCost;
                copySolution(neighbor, bestLocalSolution);
            }
        }

        if (bestNeighborCost < bestCost) {
            bestCost = bestNeighborCost;
            copySolution(bestLocalSolution, bestSolution);
            stagnationCount = 0; // Reset stagnation counter when best solution improves
        } else {
            ++stagnationCount; // Increment stagnation counter
        }

        // Terminate if 10 iterations have passed without improvement
        if (stagnationCount >= 1000) {
            break;
        }

        updateTabuList(bestLocalSolution);
        copySolution(bestLocalSolution, currentSolution);
        delete[] neighbor;
        ++iteration;
    }

    copySolution(bestSolution, optimalSolution);
    optimalCost = bestCost;

    delete[] currentSolution;
    delete[] bestLocalSolution;
}

