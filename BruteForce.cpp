#include "BruteForce.h"

void BruteForce::tspHelper(int currentNode, int currentCost, int level) {
    if (level == n) {
        if (graph[currentNode][startNode] >= 0) {  // Ignore negative edges
            int totalCost = currentCost + graph[currentNode][startNode];
            if (totalCost < minCost) {
                minCost = totalCost;
            }
        }
        return;
    }

    for (int i = 0; i < n; i++) {
        if (!visited[i] && graph[currentNode][i] >= 0) {  // Ignore negative edges
            visited[i] = true;
            tspHelper(i, currentCost + graph[currentNode][i], level + 1);
            visited[i] = false;
        }
    }
}

int BruteForce::solveTSP(int startNode) {
    this->startNode = startNode;
    minCost = INF;
    
    // Manually reset the visited array to false
    for (int i = 0; i < n; ++i) {
        visited[i] = false;
    }
    
    visited[startNode] = true;
    tspHelper(startNode, 0, 1);
    return minCost;
}
