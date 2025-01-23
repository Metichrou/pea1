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
       // cout << "3. Solve with Brute Force" << "\n";
       // cout << "4. Solve with Branch and Bound" << "\n";
        cout << "3. Solve with Tabu Search" << "\n";
        cout << "4. Solve with Simulated Annealing" << "\n";
        cout << "5. Output Adjacency Matrix" << "\n";
        cout << "6. Exit" << "\n";
        cout << "7. Solve with Dynamic Programming" << "\n";
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

        /*case 3: { // Solve with Brute Force
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
        }*/

        /*case 4: { // Solve with Branch and Bound
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
        }*/
        case 7: { // Solve with Dynamic Programming
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

        case 3: {
                int n = matrix.getSize();
                if (n == 0) {
                    std::cout << "Please load or generate a matrix first.\n";
                    break;
                }

                TS solver(matrix.getMatrix(), n, 1000); // Tenure = sizeof Tabulist
                int* optimalSolution = new int[n];
                int optimalCost;

                std::cout << "Choose neighborhood method:\n";
                std::cout << "1 - Swap Neighborhood\n";
                std::cout << "2 - Insert Neighborhood\n";
                int neighborType;
                std::cin >> neighborType;

                while (neighborType < 1 || neighborType > 2) {
                    std::cout << "Invalid choice. Please enter 1 or 2.\n";
                    std::cin >> neighborType;
                }

                // Start timer
                std::clock_t startTime = std::clock();

                // Run Tabu Search
                solver.tabuSearch(optimalSolution, optimalCost, neighborType, 120, 1000000); // 60 sec, 1000 iterations max

                // End timer
                std::clock_t endTime = std::clock();
                double elapsedSeconds = double(endTime - startTime) / CLOCKS_PER_SEC;

                // Output results
                cout << "Optimal cost: "<< optimalCost << "\n ";
                // cout  << "\nOptimal path: ";
                // for (int i = 0; i < n; ++i) {
                //     cout << optimalSolution[i] << " ";
                // }
                // cout << optimalSolution[0] << "\n"; // Closing the cycle

                // Output elapsed time
                cout << "Elapsed time: " << elapsedSeconds << " seconds\n";

                delete[] optimalSolution;
                break;
        }



        case 4: { // Solve with Simulated Annealing
                int n = matrix.getSize();
                if (n == 0) {
                    cout << "Please load or generate a matrix first." << "\n";
                    break;
                }

                // Parameters for Simulated Annealing
                double initialTemp = 1000.0 * n * n; // Initial temperature
                double coolingRate = 0.99;     // Cooling rate
                int maxIterations = 10000000;   // Max iterations
                int maxTimeSA = 120;          // Time limit in seconds (converted to microseconds)

                // Ask the user to choose a cooling method
                cout << "Choose a cooling method:\n";
                cout << "1. Logarithmic Cooling\n";
                cout << "2. Linear Cooling\n";
                cout << "3. Geometric Cooling\n";
                int coolingChoice;
                cin >> coolingChoice;

                // Validate user input
                if (coolingChoice < 1 || coolingChoice > 3) {
                    cout << "Invalid choice. Defaulting to Logarithmic Cooling.\n";
                    coolingChoice = 1;
                }

                // Create an instance of the SA class with the selected cooling method
                SA solver(matrix.getMatrix(), n, initialTemp, coolingRate, maxIterations, maxTimeSA, coolingChoice);

                int* optimalPath = new int[n];
                int bestCost;

                // Start the timer
                Timer timer;
                timer.start();

                // Run Simulated Annealing to get the best solution
                solver.simulatedAnnealing(optimalPath, &bestCost);

                // Stop the timer
                timer.stop();

                // Output the results
                 cout << "Minimum cost using Simulated Annealing: " << bestCost << "\n";
                // cout << "Optimal path: ";
                // for (int i = 0; i < n; ++i) {
                //     cout << optimalPath[i] << " ";
                // }
                // cout << optimalPath[0]; // Return to start node
                // cout << "\n";

                // Clean up
                delete[] optimalPath;

                // Print elapsed time
                timer.printElapsed("Simulated Annealing");
                break;
        }


    
        case 5:
            if (matrix.getSize() == 0) {
                cout << "Matrix is empty. Please load or generate a matrix first." << "\n";
            } else {
                matrix.print();  // Print the matrix
            }
            break;

        case 6: //Exit
            cout << "Exiting the program." << "\n";
            exitProgram = true;
            break;

        default:
            cout << "Invalid choice. Please try again." << "\n";
        }
    }

    return 0;
}
