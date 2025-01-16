#include "BnB.h"

// Constructor
BnB::BnB(int** graph, int n) : graph(graph), n(n), minCost(INF), startNode(0) {
    visited = new bool[n];
    for (int i = 0; i < n; ++i) {
        visited[i] = false;
    }
}

// Destructor
BnB::~BnB() {
    delete[] visited;
}

// Helper to find the minimum edge cost for a node
int BnB::findMinEdgeCost(int node) {
    int minCost = INF;
    for (int i = 0; i < n; ++i) {
        if (i != node && graph[node][i] >= 0) { // Ignore negative or self-loops
            if (graph[node][i] < minCost) {
                minCost = graph[node][i];
            }
        }
    }
    return minCost == INF ? 0 : minCost; // If no valid edge, return 0
}

// Lower bound calculation
int BnB::calculateLowerBound(int currentCost, int level) {
    int bound = currentCost;

    // Add the minedge cost for each unvisited node
    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            int minEdgeCost = findMinEdgeCost(i);
            bound += minEdgeCost;
        }
    }
    return bound;
}

// Recursive helper for Branch and Bound
void BnB::tspHelper(int currentNode, int currentCost, int level, int* path, int* bestPath) {
    if (level == n) {
        if (graph[currentNode][startNode] >= 0) {
            int totalCost = currentCost + graph[currentNode][startNode];
            if (totalCost < minCost) {
                minCost = totalCost;
                for (int i = 0; i < n; ++i) {
                    bestPath[i] = path[i];
                }
            }
        }
        return;
    }

    for (int i = 0; i < n; ++i) {
        if (!visited[i] && graph[currentNode][i] >= 0) {
            int nextCost = currentCost + graph[currentNode][i];
            int bound = calculateLowerBound(nextCost, level + 1);

            if (bound < minCost) { // semi upper bound
                visited[i] = true;
                path[level] = i;
                tspHelper(i, nextCost, level + 1, path, bestPath);
                visited[i] = false; 
            }
        }
    }
}

// Method to solve the TSP
int BnB::solveTSP(int startNode, int* bestPath) {
    this->startNode = startNode;
    minCost = INF;

    // Initialize the visited array and path array
    for (int i = 0; i < n; ++i) {
        visited[i] = false;
        bestPath[i] = -1;
    }

    // Start the recursive Branch and Bound process
    visited[startNode] = true;
    int* path = new int[n];
    path[0] = startNode;
    tspHelper(startNode, 0, 1, path, bestPath);

    delete[] path;
    return minCost;
}
