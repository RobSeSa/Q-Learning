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
int MAX_ROWS = 11; // default values
int MAX_COLS = 11;

string actions[4] = {"up", "right", "down", "left"};
int NUM_ACTIONS = 4;

struct State {
    int row;
    int col;
};

// helper functions
// check if given row/col is a terminal state
bool is_terminal(int **values, int row, int col) {
    int nonterminal_value = -1;
    if (values[row][col] == nonterminal_value) {
        return false;
    }
    return true;
}

// get a viable random start tuple
State get_rand_start(int **values){
    int row = rand() % MAX_ROWS;
    int col = rand() % MAX_COLS;
    while(is_terminal(values, row, col)) {
        // find new start
        row = rand() % MAX_ROWS;
        col = rand() % MAX_COLS;
    }
    struct State state = {row, col};
    return state;
}

// get the next action following an espilon greedy choice
int get_next_action(int ***q_table, int row, int col) {
    int action_num = -1;
    int max_index = -1;
    int index = -1;
    int max = -1;

    float rand_val = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    //cout << "rand_val: " << rand_val << "\n";
    // take the greedy action
    if(rand_val < EPSILON) {
        for(int i = 0; i < NUM_ACTIONS; i++) {
            // find the max value
            if(q_table[row][col][i] > max) {
                max = q_table[row][col][i];
                max_index = i;
            }
        }
        // save the index of the max value as the best action
        action_num = max_index;
    }
    // take a random action
    else {
        action_num = rand() % 4;
    }
    if(action_num == -1) {
        // still unset so error
        cerr << "action_num still unset: error in get_next_action()\n";
        return -1;
    }
    //cout << "returning action_num: " << action_num << "\n";
    return action_num;
}

// get the next location. if invalid, don't move
State get_next_location(int old_row, int old_col, int action_num) {
    // check U, D, L, R
    int new_row = old_row;
    int new_col = old_col;
    if (actions[action_num] == "up" && old_row > 0) {
        new_row = old_row - 1;
    }
    else if(actions[action_num] == "right" && old_col < MAX_COLS - 1) {
        new_col = old_col + 1;
    }
    else if(actions[action_num] ==  "down" && old_row < MAX_ROWS - 1) {
        new_row = old_row + 1;
    }
    else if(actions[action_num] == "left" && old_col > 0) {
        new_col = old_col - 1;
    }
    // else invalid move -> dont move
    State return_state = {new_row, new_col};
    return return_state;
}

 
// Dynamically Allocate Memory for 3D Array in C++
int main()
{
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

    // testing CSV reading
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

    // testing getrandstart
    State start_state = get_rand_start(values);
    cout << "Start state: (" << start_state.row << ", "
                             << start_state.col << ")\n";
    start_state = get_rand_start(values);
    cout << "Start state: (" << start_state.row << ", "
                             << start_state.col << ")\n";
    start_state = get_rand_start(values);
    cout << "Start state: (" << start_state.row << ", "
                             << start_state.col << ")\n";

    // create q_table
    int ***q_table= new int**[MAX_ROWS];
    for (int i = 0; i < MAX_ROWS; i++){
       q_table[i] = new int*[MAX_COLS];
       for (int j = 0; j < MAX_COLS; j++){
           q_table[i][j] = new int[NUM_ACTIONS];
           for (int k = 0; k < NUM_ACTIONS; k++){
              q_table[i][j][k] = 0;
           }
       }
    }

    // testing get_next_action
    int next_action = get_next_action(q_table, 0, 0);
    cout << "next action: " << next_action << "\n";

    // testing get_next_location
    State next_state = get_next_location(0, 0, 1);
    cout << "Next state: (" << next_state.row << ", "
                            << next_state.col << ")\n";
    next_state = get_next_location(next_state.row, next_state.col, 0);
    cout << "Next state: (" << next_state.row << ", "
                            << next_state.col << ")\n";





    // deallocate memory for values and q_table
    for (int i = 0; i < MAX_ROWS; i++) {
        delete[] values[i];
    }
    delete[] values;

    for (int i = 0; i < MAX_ROWS; i++) {
        for (int j = 0; j < MAX_COLS; j++) {
            delete [] q_table[i][j];
        }
        delete [] q_table[i];
    }
    delete [] q_table;
 
    return 0;
}