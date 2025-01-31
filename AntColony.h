#pragma once
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <chrono>

class AntColony {
private:
    int n;                              // Number of cities
    int** graph;                        // Adjacency matrix (ATSP graph)
    double** pheromones;                // Pheromone matrix
    double alpha = 1.0;                 // Influence of pheromones
    double beta = 2.0;                  // Influence of heuristic information
    int numAnts;                        // Number of ants
    double pheromoneAmount;             // Pheromone left by ants
    double evaporationRate;             // Evaporation rate
    double maxTime;                     // Maximum runtime in seconds
    const double INF = 1e9;

    // Initialize pheromone matrix
    void initializePheromones();

    // Construct a tour for an ant
    void constructSolution(int startNode, int* path, double& cost);

    // Select next city based on probability
    int selectNextCity(int currentNode, bool* visited);

    // Update pheromones based on ants' tours
    void updatePheromones(int** allPaths, double* allCosts);

    // Heuristic value (1/distance)
    double heuristic(int i, int j);

public:
    AntColony(int** graph, int n, int numAnts, double pheromoneAmount, double evaporationRate, double maxTime);
    ~AntColony();
    int solveATSP(int startNode, int* bestPath);
};
