#include "DP.h"

// Constructor
DP::DP(int** graph, int n) : graph(graph), n(n) {
    // Allocate memory for dp and path arrays
    dp = new int*[1 << n]; // 2^n
    path = new int*[1 << n];
    for (int i = 0; i < (1 << n); ++i) {
        dp[i] = new int[n];
        path[i] = new int[n];
        for (int j = 0; j < n; ++j) {
            dp[i][j] = INF;  // Initialize dp table to "infinity"
            path[i][j] = -1; // Initialize path table to -1
        }
    }
}

// Destructor
DP::~DP() {
    for (int i = 0; i < (1 << n); ++i) {
        delete[] dp[i];
        delete[] path[i];
    }
    delete[] dp;
    delete[] path;
}

// Solves the TSP and returns the minimum cost
int DP::solveTSP(int startNode, int* optimalPath) {
    dp[1 << startNode][startNode] = 0; // Starting at the startNode with only it visited

    // Iterate over all subsets of nodes
    for (int mask = 0; mask < (1 << n); ++mask) {
        for (int u = 0; u < n; ++u) {
            if (!(mask & (1 << u))) continue; // Skip if 'u' is not in the subset

            for (int v = 0; v < n; ++v) {
                if (mask & (1 << v)) continue; // Skip if 'v' is already visited
                if (graph[u][v] >= 0) { // Ignore negative edges
                    int nextMask = mask | (1 << v); // Add 'v' to the subset
                    int newCost = dp[mask][u] + graph[u][v];

                    // Update dp and path tables
                    if (newCost < dp[nextMask][v]) {
                        dp[nextMask][v] = newCost;
                        path[nextMask][v] = u;
                    }
                }
            }
        }
    }

    // Find the minimum cost to return to the start node
    int endMask = (1 << n) - 1; // All nodes visited
    int minCost = INF;
    int lastNode = -1;
    for (int u = 0; u < n; ++u) {
        if (dp[endMask][u] + graph[u][startNode] < minCost) {
            minCost = dp[endMask][u] + graph[u][startNode];
            lastNode = u;
        }
    }

    // Reconstruct the optimal path
    if (lastNode != -1) {
        int index = 0;
        reconstructPath(endMask, lastNode, optimalPath, index);
        optimalPath[index++] = startNode; // Return to the start node
    }

    return minCost;
}

// Helper function to reconstruct the path
void DP::reconstructPath(int mask, int currentNode, int* optimalPath, int& index) {
    if (mask == (1 << currentNode)) {
        optimalPath[index++] = currentNode;
        return;
    }
    int prevNode = path[mask][currentNode];
    reconstructPath(mask ^ (1 << currentNode), prevNode, optimalPath, index);
    optimalPath[index++] = currentNode;
}
