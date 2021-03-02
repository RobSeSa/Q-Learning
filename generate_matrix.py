# python script to produce the environment for a Q-learning algorithm to learn in
# format: (epsilon, discount factor, learning rate, dimension x, dimension, y, a[x=0][y=0], ... , a[x=x][y=y])

import numpy as np
import csv

# set hyper parameters
print("Epsilon:")
epsilon = float(input())
print("Discount Factor:")
discount_factor = float(input())
print("Learning Rate:")
learning_rate = float(input())
#print("Matrix width:")
#width = int(input())
width = 11
#print("Matrix height:")
#height= int(input())
height = 11

# print for verification
parameters = []
parameters.append(epsilon)
parameters.append(discount_factor)
parameters.append(learning_rate)
parameters.append(width)
parameters.append(height)

for param in parameters:
    print(param)

# generate a maze
def gen_maze(height, width, parameters):
    # randomly choose end point
    matrix = np.random.rand(height, width)
    print(matrix)
    for row in range(height):
        for col in range(width):
            parameters.append(matrix[row][col])
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
premade = premade_maze()

print("premade[5][5] =", premade[5][5])
for row in range(height):
        for col in range(width):
            parameters.append(premade[row][col])

#gen_maze(parameters)

# csv one line
# store in a csv
#data = np.asarray(parameters)
#data.tofile('matrix_data.csv',sep=',')

# csv multiple lines
with open('matrix_data.csv', 'w') as csv_file:
    csv_writer = csv.writer(csv_file, delimiter=',')
    csv_writer.writerow(parameters)
    for row in range(height):
        csv_writer.writerow(premade[row])