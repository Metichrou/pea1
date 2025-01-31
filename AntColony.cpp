#include "AntColony.h"

// Constructor
AntColony::AntColony(int** graph, int n, int numAnts, double pheromoneAmount, double evaporationRate, double maxTime)
    : graph(graph), n(n), numAnts(numAnts), pheromoneAmount(pheromoneAmount), evaporationRate(evaporationRate), maxTime(maxTime) {
    
    pheromones = new double*[n];
    for (int i = 0; i < n; ++i) {
        pheromones[i] = new double[n];
    }
    initializePheromones();
}

// Destructor
AntColony::~AntColony() {
    for (int i = 0; i < n; ++i) {
        delete[] pheromones[i];
    }
    delete[] pheromones;
}

// Initialize pheromone levels
void AntColony::initializePheromones() {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            pheromones[i][j] = 1.0;  // Initial pheromone level
        }
    }
}

// Heuristic function (1/distance)
double AntColony::heuristic(int i, int j) {
    return (graph[i][j] > 0) ? (1.0 / graph[i][j]) : 0.0;
}

// Select next city based on probability
int AntColony::selectNextCity(int currentNode, bool* visited) {
    double* probabilities = new double[n];
    double sum = 0.0;

    for (int i = 0; i < n; ++i) {
        if (!visited[i] && graph[currentNode][i] > 0) {
            probabilities[i] = pow(pheromones[currentNode][i], alpha) * pow(heuristic(currentNode, i), beta);
            sum += probabilities[i];
        } else {
            probabilities[i] = 0.0;
        }
    }

    if (sum == 0.0) {
        delete[] probabilities;
        return -1; // No valid move
    }

    double randValue = ((double) rand() / RAND_MAX) * sum;
    double cumulative = 0.0;

    for (int i = 0; i < n; ++i) {
        if (probabilities[i] > 0) {
            cumulative += probabilities[i];
            if (cumulative >= randValue) {
                delete[] probabilities;
                return i;
            }
        }
    }

    delete[] probabilities;
    return -1;
}

// Construct a solution for one ant
void AntColony::constructSolution(int startNode, int* path, double& cost) {
    bool* visited = new bool[n];
    for (int i = 0; i < n; ++i) visited[i] = false;

    path[0] = startNode;
    visited[startNode] = true;
    cost = 0.0;

    int currentNode = startNode;
    for (int step = 1; step < n; ++step) {
        int nextNode = selectNextCity(currentNode, visited);
        if (nextNode == -1) {
            cost = INF;  // Invalid path
            delete[] visited;
            return;
        }

        path[step] = nextNode;
        visited[nextNode] = true;
        cost += graph[currentNode][nextNode];
        currentNode = nextNode;
    }

    // Return to start
    if (graph[currentNode][startNode] > 0) {
        cost += graph[currentNode][startNode];
    } else {
        cost = INF;  // Incomplete cycle
    }

    delete[] visited;
}

// Update pheromones based on ants' tours
void AntColony::updatePheromones(int** allPaths, double* allCosts) {
    // Evaporation
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            pheromones[i][j] *= (1.0 - evaporationRate);
        }
    }

    // Deposit pheromones
    for (int k = 0; k < numAnts; ++k) {
        if (allCosts[k] < INF) {
            double deposit = pheromoneAmount / allCosts[k];
            for (int i = 0; i < n - 1; ++i) {
                pheromones[allPaths[k][i]][allPaths[k][i + 1]] += deposit;
            }
            pheromones[allPaths[k][n - 1]][allPaths[k][0]] += deposit;
        }
    }
}

// Solve ATSP using ACO based on max time
int AntColony::solveATSP(int startNode, int* bestPath) {
    int** allPaths = new int*[numAnts];
    double* allCosts = new double[numAnts];
    double bestCost = INF;

    for (int i = 0; i < numAnts; ++i) {
        allPaths[i] = new int[n];
    }

    srand(time(0));
    auto startTime = std::chrono::high_resolution_clock::now();

    while (true) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsedTime = currentTime - startTime;

        if (elapsedTime.count() >= maxTime) {
            break;  // Stop if max time reached
        }

        for (int k = 0; k < numAnts; ++k) {
            constructSolution(startNode, allPaths[k], allCosts[k]);
            if (allCosts[k] < bestCost) {
                bestCost = allCosts[k];
                for (int i = 0; i < n; ++i) {
                    bestPath[i] = allPaths[k][i];
                }
            }
        }
        updatePheromones(allPaths, allCosts);
    }

    for (int i = 0; i < numAnts; ++i) {
        delete[] allPaths[i];
    }
    delete[] allPaths;
    delete[] allCosts;

    return bestCost;
}
