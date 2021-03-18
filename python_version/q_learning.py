from IPython import get_ipython
# main script for reading the matrix csv file and learning off of the given matrix
# directly from https://realpython.com/python-csv/#reading-csv-files-with-csv
import csv
import numpy as np

# reading the csv data
data = []
with open('../matrix_data.csv') as csv_file:
    csv_reader = csv.reader(csv_file, delimiter=',')
    line_count = 0
    for row in csv_reader:
        #print(row)
        for col in row:
            data.append(col)

epsilon = float(data[0])
discount_factor = float(data[1])
learning_rate = float(data[2])
width = int(float(data[3]))
height = int(float(data[4]))

# printing hyper parameters
#print("Epsilon:", epsilon)
#print("Discount factor:", discount_factor)
#print("Learning rate:", learning_rate)
#print("Rows:", height)
#print("Columns:", width)

values = np.zeros((height, width))

index = 5 # start at index 5 to skip parameters
for row in range(height):
    for col in range(width):
        #print(data[index])
        values[row][col] = data[index]
        index += 1

actions = ['up', 'right', 'down', 'left']
num_actions = 4
# Start of Q-Learning implementation
# define helper functions

# checks if the given row, col state is terminal
def is_terminal(values, row, col):
    # -1 defined as the value of squares that are non-terminal
    nonterminal_value = -1
    if values[row][col] == nonterminal_value:
        return False
    else:
        return True

# gets a random valid start state given max rows and cols
def get_rand_start(values, max_rows, max_cols):
    start_row = np.random.randint(max_rows)
    start_col = np.random.randint(max_cols)
    while is_terminal(values, start_row, start_col):
        start_row = np.random.randint(max_rows)
        start_col = np.random.randint(max_cols)
    return start_row, start_col

# get the next action following an epsilon greedy choice
def get_next_action(q_table, epsilon, row, col):
    # return greedy action
    if np.random.random() < epsilon:
        action_num = np.argmax(q_table[row][col])
    # return random action
    else:
        action_num = np.random.randint(num_actions)
    return action_num

# get the next location. if invalid, don't move
def get_next_location(old_row, old_col, max_rows, max_cols, action_num):
    # check U, D, L, R
    new_row = old_row
    new_col = old_col
    action_name = actions[action_num]
    if action_name == 'up' and old_row > 0:
        new_row = old_row - 1
    elif action_name == 'right' and old_col < max_cols - 1:
        new_col = old_col + 1
    elif action_name == 'down' and old_row < max_rows - 1:
        new_row = old_row + 1
    elif action_name == 'left' and old_col > 0:
        new_col = old_col - 1
    # else invalid move -> dont move
    return new_row, new_col

# get the best path following the Q-table
def get_best_path(q_table, values, start_row, start_col):
    path = []
    curr_row, curr_col = start_row, start_col
    # keep appending while not terminal
    while not is_terminal(values, curr_row, curr_col):
        path.append([curr_row, curr_col])
        action_num = get_next_action(q_table, 1, curr_row, curr_col)
        curr_row, curr_col = get_next_location(curr_row, curr_col, height, width, action_num)
    # append the terminal state as well
    path.append([curr_row, curr_col])
    return path

def get_path_cost(values, path):
    cost = 0
    for row, col in path:
        cost += values[row][col]
    return cost

# define Q-table
q_table = np.zeros((height, width, num_actions))

# do the training
# from example cited in README: https://www.youtube.com/watch?v=iKdlKYG78j4&t=158s
for episode in range(1000):
    row, col = get_rand_start(values, height, width)
    # take actions from this random start state until we terminate
    while not is_terminal(values, row, col):
        # get an action
        action_num = get_next_action(q_table, epsilon, row, col)
        # perform the action
        old_row, old_col = row, col
        row, col = get_next_location(row, col, height, width, action_num)
        # receive the reward
        reward = values[row, col]
        # calculate temporal difference›
        old_q_value = q_table[old_row, old_col, action_num]
        temporal_diff = reward + (discount_factor * np.max(q_table[row][col])) - old_q_value
        # update Q-value
        new_q_value = old_q_value + (learning_rate * temporal_diff)
        q_table[old_row][old_col][action_num] = new_q_value

# get the best path using the q-table
path = get_best_path(q_table, values, 8, 8)
#print(path)
# fix the format of the path
for (x, y) in path:
    print("({}, {}) ".format(x, y), end="")
print()
cost = get_path_cost(values, path)
print(cost)