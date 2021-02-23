#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define NUM_ACTIONS 4
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
        printf("convert_index(): converted row: %d, col: %d, depth: %d to index: %d\n", row, col, depth, index);
    }
    else if(dimensions == 2) {
        index = col + (row * col);
        printf("convert_index(): converted row: %d, col: %d to index: %d\n", row, col, index);
    }
    else {
        index = -1;
        perror("invalid number of dimensions");
    }
    return index;
}

// checks if the given row, col state is terminal
bool is_terminal(int values[], int row, int col);

// gets a random valid start state given max rows and cols
state get_rand_start(int values[], int max_rows, int max_cols);
// def get_rand_start(values, max_rows, max_cols):
//     start_row = np.random.randint(max_rows)
//     start_col = np.random.randint(max_cols)
//     while is_terminal(values, start_row, start_col):
//         start_row = np.random.randint(max_rows)
//         start_col = np.random.randint(max_cols)
//     return start_row, start_col

// get the next action following an epsilon greedy choice
int get_next_action(int q_table[], double epsilon, int row, int col) {
    int action_num, max_index, max, index;
    action_num = max_index = max = -1;

    float rand_val = (double)rand() / (double)RAND_MAX;
    printf("random value in range 0 -> 1: %f\n", rand_val);
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
        printf("greedy action for row: %d, col: %d is: %d\n", row, col, action_num);
    }
    // take a random action
    else {
        action_num = rand() % 4;
        printf("random action for row: %d, col: %d is: %d\n", row, col, action_num);;
    }
    if(action_num == -1) {
        // still unset so error
        perror("Error finding best action to take in get_next_action()\n");
        return -1;
    }
    return action_num;
}
// def get_next_action(q_table, epsilon, row, col):
//     # return greedy action
//     if np.random.random() < epsilon:
//         action_num = np.argmax(q_table[row][col])
//     # return random action
//     else:
//         action_num = np.random.randint(num_actions)
//     return action_num

// get the next location. if invalid, don't move
state get_next_location(int old_row, int old_col, int max_rows, int max_cols, int action_num);
// def get_next_location(old_row, old_col, max_rows, max_cols, action_num):
//     # check U, D, L, R
//     new_row = old_row
//     new_col = old_col
//     action_name = actions[action_num]
//     if action_name == 'up' and old_row > 0:
//         new_row = old_row - 1
//     elif action_name == 'right' and old_col < max_cols - 1:
//         new_col = old_col + 1
//     elif action_name == 'down' and old_row < max_rows - 1:
//         new_row = old_row + 1
//     elif action_name == 'left' and old_col > 0:
//         new_col = old_col - 1
//     # else invalid move -> dont move
//     return new_row, new_col

// get the best path following the Q-table
state *get_best_path(int **q_table[], int values[], int start_row, int start_col);
// def get_best_path(q_table, values, start_row, start_col):
//     path = []
//     curr_row, curr_col = start_row, start_col
//     # keep appending while not terminal
//     while not is_terminal(values, curr_row, curr_col):
//         path.append([curr_row, curr_col])
//         action_num = get_next_action(q_table, 1, curr_row, curr_col)
//         curr_row, curr_col = get_next_location(curr_row, curr_col, height, width, action_num)
//     # append the terminal state as well
//     path.append([curr_row, curr_col])
//     return path

int get_path_cost(int values[], state *path);
//     cost = 0
//     for row, col in path:
//         cost += values[row][col]
//     return cost



int main() {
    FILE *matrix_data = fopen("../matrix_data.csv", "r");
    if (matrix_data == NULL) {
        perror("Error opening file matrix_data.csv");
        exit(1);
    }

    char buffer[1024];

    int temp;
    int row = 0;
   
    //int index = 0;
    
    // hyper parameters
    double epsilon, discount_factor, learning_rate, width, height;
    int param_num = 0;
    char *ptr;
    printf("Printing contents of matrx_data.csv\n");
    while(fgets(buffer, sizeof(buffer), matrix_data)) {
        char *token;

        temp = 0;
        token = strtok(buffer, ",");
        while(token != NULL) {
            if(temp != 0) {
                printf(",");
            }
            temp += 1;
            printf("%s", token);

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
                // initialize the matrix given height and width

            }

            token = strtok(NULL, ",");
        }
    }

    // testing hyper parameters
    printf("\nTesting hyper parameters:\n%f, %f, %f, %f, %f\n", epsilon, discount_factor, learning_rate, width, height);

    // setting and testing actions
    const char *actions[] = {"up", "right", "down", "left"};
    printf("Testing an action in actions array:\n%s\n", actions[0]);

    // testing state struct
    state my_state = {0, 0};
    printf("Testing row, col of a state:\n%d, %d\n", my_state.row, my_state.col);

    // used for epsilon
    printf("rand: %d\nRAND_MAX: %d\n", rand(), RAND_MAX);
    float rand_val = (double)rand() / (double)RAND_MAX;
    printf("random value in range 0 -> 1: %f\n", rand_val);

    // initialize 3D q_table: dim = (height, width, NUM_ACTIONS)
    size_t nbytes = height * width * NUM_ACTIONS * sizeof(int);
    // use flat 1D array for q_table implementation
    int *q_table = (int*) malloc(nbytes);
    memset(q_table, 0, nbytes);

    // test get_next_action()
    int action_num = get_next_action(q_table, epsilon, 0, 0);
    printf("action_num = %d\n", action_num);
    q_table[convert_index(3, 0, 0, 1)] = 10;
    action_num = get_next_action(q_table, 1, 0, 0);
    if(action_num == 1) {
        printf("get_next_action test passed");
    }
    else {
        printf("get_next_action test failed");
    }

    return 0;
}
