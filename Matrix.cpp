#include "Matrix.h"
#include <fstream>
#include <iostream>
#include <iomanip>  // For clear output
#include <random>   // For rng

using namespace std;

// Constructor
Matrix::Matrix(int size) : size(size), matrix(nullptr) {
    if (size > 0) {
        matrix = new int*[size];
        for (int i = 0; i < size; ++i) {
            matrix[i] = new int[size];
        }
    }
}

// Destructor
Matrix::~Matrix() {
    freeMatrix();
}

// Free matrix memory
void Matrix::freeMatrix() {
    if (matrix != nullptr) {
        for (int i = 0; i < size; ++i) {
            delete[] matrix[i];
        }
        delete[] matrix;
        matrix = nullptr;
        size = 0;
    }
}

// Load matrix from a file
void Matrix::loadFromFile(const string& filename) {
    freeMatrix();  // Free any existing matrix

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        return;
    }

    file >> size;  // Read matrix size
    if (size <= 0) {
        cerr << "Error: Invalid matrix size in file." << endl;
        file.close();
        return;
    }

    // Allocate memory for the matrix
    matrix = new int*[size];
    for (int i = 0; i < size; ++i) {
        matrix[i] = new int[size];
    }

    // Read matrix values from the file
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            file >> matrix[i][j];
        }
    }

    file.close();
}

// Generate a random matrix
void Matrix::generateRandom(int newSize, int min, int max) {
    freeMatrix();  // Free any existing matrix

    size = newSize;
    matrix = new int*[size];
    for (int i = 0; i < size; ++i) {
        matrix[i] = new int[size];
    }

    // Random number generator
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(min, max);

    // Populate the matrix with random values
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (i == j){
                matrix[i][j] = -1;
            } else 
            matrix[i][j] = dis(gen);
        }
    }
    cout << "Random matrix generated successfully." << endl;
}

// Print the matrix
void Matrix::print() const {
    if (size == 0 || matrix == nullptr) {
        cout << "Matrix is empty." << endl;
        return;
    }

    cout << "Matrix (" << size << "x" << size << "):" << endl;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            cout << setw(5) << matrix[i][j] << " ";  // Formatted output for better readability
        }
        cout << endl;
    }
}

// Get matrix size
int Matrix::getSize() const {
    return size;
}

// Get the raw matrix pointer
int** Matrix::getMatrix() const {
    return matrix;
}

// Get value at (row, col)
int Matrix::getValue(int row, int col) const {
    if (row >= 0 && row < size && col >= 0 && col < size) {
        return matrix[row][col];
    }
    cerr << "Error: Index out of bounds." << endl;
    return -1;  // Indicate an error
}

