# python script to produce the environment for a Q-learning algorithm to learn in
# format: (epsilon, discount factor, learning rate, dimension x, dimension, y, a[x=0][y=0], ... , a[x=x][y=y])

import numpy as np
import csv
import random

# set hyper parameters
print()
print("====================================================================")
print("Epsilon: ", end="")
epsilon = float(input())
print(epsilon)
print("Discount Factor: ", end="")
discount_factor = float(input())
print(discount_factor)
print("Learning Rate: ", end="")
learning_rate = float(input())
print(learning_rate)
print("Matrix width: ", end="")
width = int(input())
print(width)
print("Matrix height: ", end="")
height= int(input())
print(height)

# print for verification
parameters = []
parameters.append(epsilon)
parameters.append(discount_factor)
parameters.append(learning_rate)
parameters.append(width)
parameters.append(height)

#for param in parameters:
#    print(param)

# generate a maze
# sides are -100
# one side square is +100
# insides are randomly -100 || -1
def gen_maze(height, width):
    # randomly choose end point
    matrix = np.full((width, height), -100)
    # set the middle of the first row to the reward square
    matrix[0, width//2] = 100
    # randomly set %XX of the inner squares to -1 (traversable squares)
    # these are squares from 1->width-2; 1->height-2
    for _ in range((int) ((height*width) * 0.8)):
        row = random.randint(1, height-2)
        col = random.randint(1, width-2)
        matrix[row][col] = -1
    # set the square directly below 100 square to -1
    matrix[1, width//2] = -1

    # set the 9, 9 square to -1 bc all my algorithms will start from there
    matrix[8,8] = -1
    
    return matrix

# this function is directly from https://www.youtube.com/watch?v=iKdlKYG78j4&t=158s
def premade_maze():
    #Create a 2D numpy array to hold the rewards for each state. 
    #The array contains 11 rows and 11 columns (to match the shape of the environment), and each value is initialized to -100.
    rewards = np.full((width, height), -100)
    rewards[0, 5] = 100 #set the reward for the packaging area (i.e., the goal) to 100

    #define aisle locations (i.e., white squares) for rows 1 through 9
    aisles = {} #store locations in a dictionary
    aisles[1] = [i for i in range(1, 10)]
    aisles[2] = [1, 7, 9]
    aisles[3] = [i for i in range(1, 8)]
    aisles[3].append(9)
    aisles[4] = [3, 7]
    aisles[5] = [i for i in range(11)]
    aisles[6] = [5]
    aisles[7] = [i for i in range(1, 10)]
    aisles[8] = [3, 7]
    aisles[9] = [i for i in range(11)]

    #set the rewards for all aisle locations (i.e., white squares)
    for row_index in range(1, 10):
      for column_index in aisles[row_index]:
        rewards[row_index, column_index] = -1.
    
    #print rewards matrix
    for row in rewards:
      print(row)
    
    return rewards


# premade maze has size 11
#premade = premade_maze()

#print("premade[5][5] =", premade[5][5])
#for row in range(height):
#        for col in range(width):
#            parameters.append(premade[row][col])

#gen_maze(parameters)

# csv one line
# store in a csv
#data = np.asarray(parameters)
#data.tofile('matrix_data.csv',sep=',')

# test gen_maze
matrix = gen_maze(height, width)
print("Randomly generated matrix:")
print(matrix)

# csv multiple lines (main)
file_head = 'matrix_data'
ending = '.csv'
file_name = file_head + str(height) + 'x' + str(width) + ending
#print("Generated file name: ", file_name)

# ^ it is too much work renaming the file in each implementation
file_name = file_head + ending

with open(file_name, 'w') as csv_file:
    csv_writer = csv.writer(csv_file, delimiter=',')
    csv_writer.writerow(parameters)
    for row in range(height):
        csv_writer.writerow(matrix[row])