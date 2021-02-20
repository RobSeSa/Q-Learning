#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// declare a pair called state for passing row and col of a given state
typedef struct state {
    int row, col;
} state;

// helper functions
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
// 
// get the next action following an epsilon greedy choice
int get_next_action(int q_table[][][], double epsilon, int row, int col);
// def get_next_action(q_table, epsilon, row, col):
//     # return greedy action
//     if np.random.random() < epsilon:
//         action_num = np.argmax(q_table[row][col])
//     # return random action
//     else:
//         action_num = np.random.randint(num_actions)
//     return action_num
// 
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
// 
// get the best path following the Q-table
state *get_best_path(int q_table[][][], int values[], int start_row, int start_col);
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
// 
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
                printf("Adding parameter");
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
                       break;
                    case 4 :
                       height = strtod(token, &ptr);
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
    printf("%f, %f, %f, %f, %f\n", epsilon, discount_factor, learning_rate, width, height);

    // setting and testing actions
    const char *actions[] = {"up", "right", "down", "left"};
    printf("%s\n", actions[0]);

    // testing state struct
    state my_state = {0, 0};
    printf("%d, %d\n", my_state.row, my_state.col);

    return 0;
}
