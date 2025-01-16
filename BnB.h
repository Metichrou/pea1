#pragma once

class BnB {
private:
    int n;               // Number of nodes
    int** graph;         // Pointer to adjacency matrix
    bool* visited;       // Dynamically allocated array to track visited nodes
    int minCost;         // Minimum cost of TSP
    int startNode;       // Starting node
    const int INF = 1000000; // Custom "infinity" value (instead of INT_MAX)

    // Helper method for the branch and bound algorithm
    void tspHelper(int currentNode, int currentCost, int level, int* path, int* bestPath);

    // Method to calculate the lower bound for a partial path
    int calculateLowerBound(int currentCost, int level);

    // Helper to find the minimum edge cost for a node
    int findMinEdgeCost(int node);

public:
    // Constructor
    BnB(int** graph, int n);

    // Destructor
    ~BnB();

    // Method to solve the TSP problem
    int solveTSP(int startNode, int* bestPath);
};
