#pragma once

class TS {
private:
    int** costMatrix;    // Cost matrix for TSP
    int size;            // Number of cities
    int tenure;          // Tabu tenure
    int** tabuList;      // Tabu list to track forbidden moves
    int* bestSolution;   // Best solution found
    int bestCost;        // Cost of the best solution

    // Initializes the solution using a simple greedy approach
    void greedyInitialization(int* solution);

    // Calculates the cost of a given solution
    int calculateCost(const int* solution);

    // Checks if a solution is tabu
    bool isTabu(const int* solution);

    // Updates the tabu list
    void updateTabuList(const int* solution);

    // Generates neighbors based on the given neighborhood type
    void generateNeighbor(const int* currentSolution, int* neighbor, int neighborType);

    // Helper functions for neighbor generation
    void generateSwapNeighbor(const int* solution, int* neighbor, int i, int j);
    void generateInsertNeighbor(const int* solution, int* neighbor, int i, int j);

    // Copies a solution from source to destination
    void copySolution(const int* source, int* destination);

public:
    // Constructor
    TS(int** costMatrix, int size, int tenure);

    // Destructor
    ~TS();

    // Executes the Tabu Search algorithm
    void tabuSearch(int* optimalSolution, int& optimalCost, int neighborType, double maxTimeTS, int maxIterations);
};
