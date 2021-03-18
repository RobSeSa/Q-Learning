#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#define NUM_ACTIONS 4
const char *actions[] = {"up", "right", "down", "left"};
int max_rows;
int max_cols;

// declare a pair called state for passing row and col of a given state
typedef struct state {
    int row, col;
} state;

// helper functions
// converting 3D/2D indices to 1D
int convert_index(int dimensions, int row, int col, int depth) { // note: max_depth always = NUM_ACTIONS
    int index;
    if(dimensions == 3) {
        index = depth + (col * NUM_ACTIONS) + (row * max_cols * NUM_ACTIONS);
        //printf("convert_index(): converted row: %d, col: %d, depth: %d to index: %d\n", row, col, depth, index);
    }
    else if(dimensions == 2) {
        index = col + (row * max_cols);
        //printf("convert_index(): converted row: %d, col: %d to index: %d\n", row, col, index);
    }
    else {
        index = -1;
        perror("invalid number of dimensions");
    }
    return index;
}

// checks if the given row, col state is terminal
bool is_terminal(int values[], state my_state) {
    // -1 defined as the value of squares that are non-terminal
    int nonterminal_value = -1;
    if(values[convert_index(2, my_state.row, my_state.col, 0)] == nonterminal_value) {
        return false;
    }
    else {
        return true;
    }
}

// gets a random valid start state given max rows and cols
state get_rand_start(int values[], int max_rows, int max_cols) {
    //printf("Getting a new random start\n");
    int r = rand() % max_rows;
    int c = rand() % max_cols;
    state my_state;
    my_state.row = r;
    my_state.col = c;
    while(is_terminal(values, my_state)) {
        // keep finding new starts until not a terminal state
        r = rand() % max_rows;
        c = rand() % max_cols;
        my_state.row = r;
        my_state.col = c;
    }
    return my_state;
}

// get the next action following an epsilon greedy choice
int get_next_action(int q_table[], double epsilon, int row, int col) {
    int action_num, max_index, max, index;
    action_num = max_index = -1;
    max = -10000;

    float rand_val = (double)rand() / (double)RAND_MAX;
    //printf("random value in range 0 -> 1: %f\n", rand_val);
    // take the greedy action
    if(rand_val < epsilon) {
        for(int i = 0; i < NUM_ACTIONS; i++) {
            // find the max value
            index = convert_index(3, row, col, i);
            if(q_table[index] > max) {
                max = q_table[index];
                max_index = i;
            }
        }
        // save the index of the max value as the best action
        action_num = max_index;
        //printf("greedy action for row: %d, col: %d is: %d\n", row, col, action_num);
    }
    // take a random action
    else {
        action_num = rand() % 4;
        //printf("random action for row: %d, col: %d is: %d\n", row, col, action_num);;
    }
    if(action_num == -1) {
        // still unset so error
        perror("Error finding best action to take in get_next_action()\n");
        return -1;
    }
    return action_num;
}

// get the next location. if invalid, don't move
state get_next_location(int old_row, int old_col, int action_num) {
    // check U, D, L, R
    int new_row = old_row;
    int new_col = old_col;
    char action_name[10]; // all the actions are < 10 letters
    strcpy(action_name, actions[action_num]);
    //printf("get_next_location(): Checking action_name: %s\n", action_name);
    if(strcmp(action_name, "up") == 0 && old_row > 0) {
        new_row = old_row - 1;
    }
    else if(strcmp(action_name, "right") == 0 && old_col < max_cols - 1) {
        new_col = old_col + 1;
    }
    else if(strcmp(action_name, "down") == 0 && old_row < max_rows - 1) {
        new_row = old_row + 1;
    }
    else if(strcmp(action_name, "left") == 0 && old_col > 0) {
        new_col = old_col - 1;
    }
    // else invalid move -> dont move
    state return_state;
    return_state.row = new_row;
    return_state.col = new_col;
    return return_state;
}

// get the best path following the Q-table
state *get_best_path(int q_table[], int values[], int start_row, int start_col) {
    // return an array of states
    state* path = (state *) malloc(100*sizeof(state));
    int path_index = 0;
    int max_path_length = 100;
    state curr_state;
    curr_state.row = start_row;
    curr_state.col = start_col;
    int action_num;
    int count = 0;
    // keep appending while not terminal state
    while(!is_terminal(values, curr_state)) {
        count++;
        if(count > 100) { break; }
        // check bounds of path_index
        if(path_index >= max_path_length) {
            // allocate more memory and update max_path_length
            max_path_length += 100;
            path = (state *) realloc(path, max_path_length);
        }
        path[path_index] = curr_state;
        action_num = get_next_action(q_table, 1, curr_state.row, curr_state.col);
        curr_state = get_next_location(curr_state.row, curr_state.col, action_num);
        path_index++;
    }
    path[path_index] = curr_state;
    return path;
}

int get_path_cost(int values[], state *path) {
    int cost = 0;
    int path_index = 0;
    state temp = path[path_index];
    while(!is_terminal(values, temp)) {
        cost += values[convert_index(2, temp.row, temp.col, 0)];
        path_index++;
        temp = path[path_index];
    }
    cost += values[convert_index(2, temp.row, temp.col, 0)];
    return cost;
}

void print_path(int values[], state *path) {
    int path_index = 0;
    state temp = path[path_index];
    while(!is_terminal(values, temp)) {
        printf("(%d, %d) ", temp.row, temp.col);
        path_index++;
        temp = path[path_index];
    }
    printf("(%d, %d) ", temp.row, temp.col);
    printf("\n");
}

// training function
void q_training(int q_table[], int values[], double epsilon, double discount_factor, double learning_rate) {
    state curr_state, old_state;
    int action_num, reward, old_q_value;
    float temp_diff, new_q_value;
    int index, max; 
    for(int episode = 0; episode < 1000; episode++) {
        curr_state = get_rand_start(values, max_rows, max_cols);
        // take actions from this random start state until we terminate 
        while(!is_terminal(values, curr_state)) {
            // get an action
            action_num = get_next_action(q_table, epsilon, curr_state.row, curr_state.col);
            // perform the action
            old_state.row = curr_state.row;
            old_state.col = curr_state.col;
            curr_state = get_next_location(curr_state.row, curr_state.col, action_num);
            // receive the reward
            reward = values[convert_index(2, curr_state.row, curr_state.col, 0)];
            // calculate temporal difference
            old_q_value = q_table[convert_index(3, old_state.row, old_state.col, action_num)];
            // find the max value
            max = -100000;
            for(int i = 0; i < NUM_ACTIONS; i++) {
                index = convert_index(3, curr_state.row, curr_state.col, i);
                if(q_table[index] > max) {
                    max = q_table[index];
                }
            }
            temp_diff = reward + (discount_factor * max) - old_q_value;
            // update Q-value
            new_q_value = old_q_value + (learning_rate * temp_diff);
            q_table[convert_index(3, old_state.row, old_state.col, action_num)] = round(new_q_value);
        }
    }
}

int main() {
    FILE *matrix_data = fopen("../matrix_data.csv", "r");
    if (matrix_data == NULL) {
        perror("Error opening file matrix_data.csv");
        exit(1);
    }

    char buffer[1024];

    int temp;
    int row = 0;
   
    
    // hyper parameters
    double epsilon, discount_factor, learning_rate, width, height;
    int param_num = 0; // for hyper parameter switch case
    // for the values matrix
    int *values;
    bool created_values_array = false;
    int index = 0;
    // for saving the tokens in the csv file
    char *ptr;
    //printf("Printing contents of matrx_data.csv\n");
    while(fgets(buffer, sizeof(buffer), matrix_data)) {
        char *token;

        temp = 0;
        token = strtok(buffer, ",");
        while(token != NULL) {
            /*
            if(temp != 0) {
                printf(",");
            }
            temp += 1;
            printf("%s", token);
            */

            // first row so add individual parameters
            if(row == 0) {
                //printf("Adding parameter");
                switch(param_num) {
                    case 0 :
                       epsilon = strtod(token, &ptr);
                       break;
                    case 1 :
                       discount_factor = strtod(token, &ptr);
                       break;
                    case 2 :
                       learning_rate = strtod(token, &ptr);
                       break;
                    case 3 :
                       width = strtod(token, &ptr);
                       max_cols = width;
                       break;
                    case 4 :
                       height = strtod(token, &ptr);
                       max_rows = height;
                       row += 1; // done with parameters so increase the row
                       break;
                }
                param_num++;
            }
            // the rest of the rows are for the matrix
            else {
                if(!created_values_array) {
                    // initialize the matrix given height and width
                    values = (int *)malloc(height * width * sizeof(int));
                    if (values == NULL) { 
                        printf("Memory not allocated.\n"); 
                        exit(1); 
                    } 
                    created_values_array = true;
                }
                values[index] = atoi(token);
                index++;
            }

            token = strtok(NULL, ",");
        }
    }

    // testing hyper parameters
    //printf("\nTesting hyper parameters:\n%f, %f, %f, %f, %f\n", epsilon, discount_factor, learning_rate, width, height);

    // initialize 3D q_table: dim = (height, width, NUM_ACTIONS)
    size_t nbytes = height * width * NUM_ACTIONS * sizeof(int);
    // use flat 1D array for q_table implementation
    int *q_table = (int*) malloc(nbytes);
    memset(q_table, 0, nbytes);

    // record the time from before q_training to immediately after
    //clock_t begin = clock();


    // start the actual training
    q_training(q_table, values, epsilon, discount_factor, learning_rate);
    state *best_path = get_best_path(q_table, values, 8, 8);
    print_path(values, best_path);
    int cost = get_path_cost(values, best_path);
    printf("%d\n", cost);

    //clock_t end = clock();
    //double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    //printf("Time spent: %f\n", time_spent);

    free(q_table);
    return 0;
}
