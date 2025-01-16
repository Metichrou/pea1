#pragma once

class DP {
private:
    int n;               // Number of nodes
    int** graph;         // Pointer to adjacency matrix
    const int INF = 1000000; // Custom "infinity" value (instead of INT_MAX)
    int** dp;            // DP table: dp[mask][i] stores the minimum cost to visit subset 'mask' ending at 'i'
    int** path;          // Path table to reconstruct the optimal path

    // Recursive helper to reconstruct the optimal path
    void reconstructPath(int mask, int currentNode, int* optimalPath, int& index);

public:
    // Constructor
    DP(int** graph, int n);

    // Destructor
    ~DP();

    // Solves the TSP and returns the minimum cost
    int solveTSP(int startNode, int* optimalPath);
};
