#pragma once


#include <iostream>
using namespace std;

class Matrix {
private:
    int** matrix;  // Pointer to the 2D matrix
    int size;      // Size of the matrix (n x n)

    // Utility function to free matrix memory
    void freeMatrix();

public:
    // Constructors and Destructor
    Matrix(int size = 0);  // Constructor with default size
    ~Matrix();            // Destructor

    // Matrix Operations
    void loadFromFile(const string& filename);  // Load matrix from a file
    void generateRandom(int size, int min = 0, int max = 100);  // Generate random matrix
    void print() const;  // Print the matrix
    int getSize() const;  // Get the matrix size
    int** getMatrix() const;  // Get the raw matrix pointer

    // Matrix manipulation
    int getValue(int row, int col) const;  // Get value at (row, col)
    void setValue(int row, int col, int value);  // Set value at (row, col)
};

