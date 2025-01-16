#pragma once

class BruteForce {
private:
    int n;               // Number of nodes
    int** graph;         // Pointer to adjacency matrix
    bool* visited;       // Dynamically allocated array to track visited nodes
    int minCost;         // Minimum cost of TSP
    int startNode;       // Starting node
    const int INF = 1000000; // Custom "infinity" value (instead of INT_MAX)

    void tspHelper(int currentNode, int currentCost, int level);

public:
    // Constructor definition directly in the header file
    BruteForce(int** graph, int n) : graph(graph), n(n), minCost(1000000), startNode(0) {
        visited = new bool[n];  // Dynamically allocate the visited array
        for (int i = 0; i < n; ++i) {
            visited[i] = false;  // Initialize visited array to false manually
        }
    }

    // Destructor declaration
    ~BruteForce() {
        delete[] visited;  // Free dynamically allocated memory
    }

    int solveTSP(int startNode);
};
