#include "Matrix.h"
#include "BruteForce.h"
#include "BnB.h" 
#include "DP.h"
#include "SA.h"
#include "TS.h"
#include "Timer.h"
#include <chrono>

using namespace std;

int main() {
    Matrix matrix;
    bool exitProgram = false;

    while (!exitProgram) {
        cout << "\n--- Asymmetric TSP Menu ---" << "\n";
        cout << "1. Load Adjacency Matrix from File" << "\n";
        cout << "2. Generate Random Matrix" << "\n";
        cout << "3. Solve with Brute Force" << "\n";
        cout << "4. Solve with Branch and Bound" << "\n";
        cout << "5. Solve with Dynamic Programming" << "\n";
        cout << "6. Solve with Tabu Search" << "\n";
        cout << "7. Solve with Simulated Annealing" << "\n";
        cout << "8. Output Adjacency Matrix" << "\n";
        cout << "9. Exit" << "\n";
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number." << "\n";
            continue;
        }

        switch (choice) {
        case 1: {
            char filename[256];
            cout << "Enter the filename: ";
            cin >> filename;

            matrix.loadFromFile(filename); 

            if (matrix.getSize() == 0) {
                cout << "Failed to load matrix from file or file is empty." << "\n";
            } else {
                cout << "Matrix loaded successfully." << "\n";
            }
            break;
        }

        case 2: {
            int size;
            cout << "Enter the size of the matrix: ";
            cin >> size;

            if (cin.fail() || size <= 0) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid matrix size. Please enter a positive integer." << "\n";
                break;
            }

            matrix.generateRandom(size, 0, 100);
            break;
        }

        case 3: { // Solve with Brute Force
            int n = matrix.getSize();
            if (n == 0) {
                cout << "Please load or generate a matrix first." << "\n";
                break;
            }

            BruteForce solver(matrix.getMatrix(), n);
            Timer timer;

            timer.start();
            int startNode = 0;
            int minCost = solver.solveTSP(startNode);
            timer.stop();

            cout << "Minimum cost using brute force: " << minCost << "\n";
            timer.printElapsed("Brute Force");
            break;
        }

        case 4: { // Solve with Branch and Bound
            int n = matrix.getSize();
            if (n == 0) {
                cout << "Please load or generate a matrix first." << "\n";
                break;
            }

            BnB solver(matrix.getMatrix(), n);  
            int* bestPath = new int[n]; // Allocate memory for the optimal path

            Timer timer;
            timer.start();

            int startNode = 0;
            int minCost = solver.solveTSP(startNode, bestPath); // Solve with BnB

            timer.stop();

            cout << "Minimum cost using Branch and Bound: " << minCost << "\n";
            cout << "Optimal path: ";
            for (int i = 0; i < n; i++) {
                cout << bestPath[i] << " ";
            }
            cout << startNode; 
            cout << "\n";

            delete[] bestPath; 

            timer.printElapsed("Branch and Bound");
            break;
        }
        case 5: { // Solve with Dynamic Programming
                int n = matrix.getSize();
                if (n == 0) {
                    cout << "Please load or generate a matrix first." << "\n";
                    break;
                }

                DP solver(matrix.getMatrix(), n); // Instantiate the solver
                int* optimalPath = new int[n + 1]; 

                Timer timer;
                timer.start();

                int minCost = solver.solveTSP(0, optimalPath); 

                timer.stop();

               
                cout << "Minimum cost using Dynamic Programming: " << minCost << "\n";
                cout << "Optimal path: ";
                for (int i = 0; i <= n; i++) { 
                    cout << optimalPath[i] << " ";
                }
                cout << "\n";

                delete[] optimalPath; // Free allocated memory for the path
                timer.printElapsed("Dynamic Programming");
                break;
        }

        case 6: { // Solve with Tabu Search
                int n = matrix.getSize();
                if (n == 0) {
                    cout << "Please load or generate a matrix first." << "\n";
                    break;
                }

                TS solver(matrix.getMatrix(), n, 100, 10); 
                int* optimalPath = new int[n];

                Timer timer;
                timer.start();

                int minCost = solver.solveTSP(optimalPath);

                timer.stop();

                
                cout << "Minimum cost using Tabu Search: " << minCost << "\n";
                cout << "Optimal path: ";
                for (int i = 0; i < n; ++i) {
                    cout << optimalPath[i] << " ";
                }
                cout << optimalPath[0]; 
                cout << "\n";

                delete[] optimalPath; 
                timer.printElapsed("Tabu Search");
                break;
        }

        case 7: { 
                int n = matrix.getSize();
                if (n == 0) {
                    cout << "Please load or generate a matrix first." << "\n";
                    break;
                }

                
                SimulatedAnnealing solver(matrix.getMatrix(), n, 1000.0, 0.01, 0.95);

                int* optimalPath = new int[n];
                int minCost = solver.solveTSP(optimalPath);

                
                cout << "Minimum cost using Simulated Annealing: " << minCost << "\n";
                cout << "Optimal path: ";
                for (int i = 0; i < n; ++i) {
                    cout << optimalPath[i] << " ";
                }
                cout << "\n";

                delete[] optimalPath;
                break;
        }
    
        case 8:
            if (matrix.getSize() == 0) {
                cout << "Matrix is empty. Please load or generate a matrix first." << "\n";
            } else {
                matrix.print();  // Print the matrix
            }
            break;

        case 9: //Exit
            cout << "Exiting the program." << "\n";
            exitProgram = true;
            break;

        default:
            cout << "Invalid choice. Please try again." << "\n";
        }
    }

    return 0;
}
