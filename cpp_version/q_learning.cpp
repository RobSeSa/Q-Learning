// C++ implementation of Q-learning on "Robots in a Warehouse"


// testing dynamic memory allocation
// from: https://www.techiedelight.com/dynamic-memory-allocation-in-c-for-2d-3d-array/
#include <iostream>
#include <sstream> // string stream
#include <vector>
#include <fstream>

using namespace std;

// global variables
double EPSILON;
double DISCOUNT_FACTOR;
double LEARNING_RATE;
int MAX_ROWS;
int MAX_COLS;

string actions[4] = {"up", "right", "down", "left"};
int num_actions = 4;
 
// Dynamically Allocate Memory for 3D Array in C++
int main()
{
    // test actions array global
    cout << "Printing actions\n";
    for(int i = 0; i < num_actions; i++)
        cout << actions[i] << "\n";

    // read X, Y, Z from file instead of input now
    string file = "../matrix_data.csv";

    // lines
    int i = 0;
    ifstream coeff(file); // opening the file
    //if (coeff.is_open()) // if the file is open
    string buff, line;
    // read the hyper parameters
    getline(coeff, buff, ',');
    EPSILON = stod(buff);
    getline(coeff, buff, ',');
    DISCOUNT_FACTOR = stod(buff);
    getline(coeff, buff, ',');
    LEARNING_RATE = stod(buff);
    getline(coeff, buff, ',');
    MAX_ROWS = stoi(buff);
    getline(coeff, buff, ',');
    MAX_COLS = stoi(buff);
    cout << "Epsilon: " << EPSILON << "\n";
    cout << "DC: " << DISCOUNT_FACTOR << "\n";
    cout << "LR: " << LEARNING_RATE << "\n";
    cout << "MAX_ROWS: " << MAX_ROWS << "\n";
    cout << "MAX_COLS: " << MAX_COLS << "\n";

    // create a 2D values matrix
    int** values = new int*[MAX_ROWS];
    // allocate memory for the columns in the row (remember to delete later)
    for(int row = 0; row < MAX_ROWS; row++) {
        values[row] = new int[MAX_COLS];
    }

    // read the rest of the matrix
    for (int row = 0; row < MAX_ROWS; row++) {
        // read one line
        getline(coeff, line);
        stringstream ss(line);
        for (int col = 0; col < MAX_COLS; col++) {
            // read csv from that line
            getline(ss, buff, ',');
            //cout << row << ", " << col << ": " << buff << "\n";
            values[row][col] = stoi(buff);
        }
        //cout << "\n";
    }

    cout << "Printing contents of values matrix!\n";
    for (int row = 0; row < MAX_ROWS; row++) {
        for (int col = 0; col < MAX_COLS; col++) {
            cout << values[row][col];
            if (col < MAX_COLS - 1) {
                cout << ", ";
            }
        }
        cout << "\n";
    }

    // deallocate memory
    for (int i = 0; i < MAX_ROWS; i++)
    {
        delete[] values[i];
    }
 
    delete[] values;
 
    return 0;
}