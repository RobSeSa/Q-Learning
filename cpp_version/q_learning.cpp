// C++ implementation of Q-learning on "Robots in a Warehouse"


// testing dynamic memory allocation
// from: https://www.techiedelight.com/dynamic-memory-allocation-in-c-for-2d-3d-array/
#include <iostream>
#include <sstream> // string stream
#include <vector>
#include <fstream>
#include <math.h>

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

// create list of States
typedef vector<State> State_list;

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
int get_next_action(int ***q_table, double epsilon, int row, int col) {
    int action_num = -1;
    int max_index = -1;
    int index = -1;
    int max = -10000;

    float rand_val = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    // take the greedy action
    if(rand_val < epsilon) {
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
        cout << "action_num still unset: error in get_next_action()\n";
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

// return a list of States for the best path in the q_table
State_list get_best_path(int ***q_table, int **values, int start_row, int start_col) {
    State_list path;
    int curr_row = start_row;
    int curr_col = start_col;
    int path_index = 0;
    int action_num;
    State next_state;
    int count =  0;
    // continue until terminal (or 100 steps)
    while(!is_terminal(values, curr_row, curr_col)) {
        count++;
        if(count > 100) { break; }
        path.push_back(State());
        path[path_index].row = curr_row;
        path[path_index].col = curr_col;
        path_index++;
        // set epsilon = 1 to take greedy action
        action_num = get_next_action(q_table, 1, curr_row, curr_col);
        next_state = get_next_location(curr_row, curr_col, action_num);
        curr_row = next_state.row;
        curr_col = next_state.col;
    }
    path.push_back(State());
    path[path_index].row = curr_row;
    path[path_index].col = curr_col;
    return path;
}

// sum path cost using values array
int get_path_cost(int **values, State_list path) {
    int cost = 0;
    for (State_list::const_iterator i = path.begin(); i != path.end(); ++i) {
        //cout << "path: (" << i->row << ", "
        //                  << i->col << ")\n";
        //cout << "cost: " << values[i->row][i->col] << "\n";
        cost += values[i->row][i->col];
    }
    return cost;
}


// helper function from printing the path format
void print_path(State_list path) {
    for (State_list::const_iterator i = path.begin(); i != path.end(); ++i) {
        cout << "(" << i->row << ", " << i->col << ") ";
    }
    cout << "\n";
}

// training function
void q_training(int ***q_table, int **values) {
    State curr_state, old_state;
    int action_num, reward, old_q_value;
    float temp_diff, new_q_value;
    int index, max; 
    for(int episode = 0; episode < 1000; episode++) {
        curr_state = get_rand_start(values);
        // take actions from this random start state until we terminate 
        while(!is_terminal(values, curr_state.row, curr_state.col)) {
            //cout << "curr_state: (" << curr_state.row << ", " << curr_state.col << ")\n";
            // get an action
            action_num = get_next_action(q_table, EPSILON, curr_state.row, curr_state.col);
            // perform the action
            old_state.row = curr_state.row;
            old_state.col = curr_state.col;
            curr_state = get_next_location(curr_state.row, curr_state.col, action_num);
            // receive the reward
            reward = values[curr_state.row][curr_state.col];
            // calculate temporal difference
            old_q_value = q_table[old_state.row][old_state.col][action_num];
            // find the max value
            max = -100000;
            for (int i = 0 ; i < NUM_ACTIONS; i++) {
                if(q_table[curr_state.row][curr_state.col][i] > max) {
                    max = q_table[curr_state.row][curr_state.col][i];
                }
            }
            temp_diff = reward + (DISCOUNT_FACTOR * max) - old_q_value;
            //cout << "temporal difference: " << temp_diff << " = " << reward << " + (" << DISCOUNT_FACTOR
            //     << " * " << max << ") - " << old_q_value << "\n";
            // update Q-value
            new_q_value = old_q_value + (LEARNING_RATE * temp_diff);
            q_table[old_state.row][old_state.col][action_num] = round(new_q_value);
        }
    }
}
 
// Dynamically Allocate Memory for 3D Array in C++
int main()
{
    string file = "../matrix_data.csv";
    int start_row = 8;
    int start_col = 8;

    // lines
    int i = 0;
    ifstream coeff(file); // opening the file
    //if (coeff.is_open()) // if the file is open
    string buff, line;
    // read the hyper parameters
    getline(coeff, line);
    stringstream ss(line);
    getline(ss, buff, ',');
    EPSILON = stod(buff);
    getline(ss, buff, ',');
    DISCOUNT_FACTOR = stod(buff);
    getline(ss, buff, ',');
    LEARNING_RATE = stod(buff);
    getline(ss, buff, ',');
    MAX_ROWS = stoi(buff);
    getline(ss, buff, ',');
    MAX_COLS = stoi(buff);
    /*
    cout << "Epsilon: " << EPSILON << "\n";
    cout << "DC: " << DISCOUNT_FACTOR << "\n";
    cout << "LR: " << LEARNING_RATE << "\n";
    cout << "MAX_ROWS: " << MAX_ROWS << "\n";
    cout << "MAX_COLS: " << MAX_COLS << "\n";
    */

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

    // call training function
    q_training(q_table, values);
    State_list best_path = get_best_path(q_table, values, start_row, start_col);
    print_path(best_path);
    int cost = get_path_cost(values, best_path);
    cout << cost << "\n";


    // testing q_training (check contents of q_table)
    /*
    cout << "Printing q_table:\n";
    for (int i = 0; i < MAX_ROWS; i++){
        cout << "row " << i << ":\n";
        for (int j = 0; j < MAX_COLS; j++){
            cout << "col " << j << ": ";
            for (int k = 0; k < NUM_ACTIONS; k++){
                cout << q_table[i][j][k] << ", ";
            }
            cout << "\n";
        }
        cout << "\n";
    }

    cout << "Printing contents of values matrix!\n";
    for (int row = 0; row < MAX_ROWS; row++) {
        for (int col = 0; col < MAX_COLS; col++) {
            cout << values[row][col];
            if (col < MAX_COLS - 1) {
                cout << ",";
            }
        }
        cout << "\n";
    }*/


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