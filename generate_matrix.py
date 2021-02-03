# python script to produce the environment for a Q-learning algorithm to learn in
# format: (epsilon, discount factor, learning rate, dimension x, dimension, y, a[x=0][y=0], ... , a[x=x][y=y])

import numpy as np

print("Epsilon:")
epsilon = float(input())
print("Discount Factor:")
discount_factor = float(input())
print("Learning Rate:")
learning_rate = float(input())
print("Matrix width:")
width = int(input())
print("Matrix height:")
height= int(input())

# print for verification
parameters = []
parameters.append(epsilon)
parameters.append(discount_factor)
parameters.append(learning_rate)
parameters.append(width)
parameters.append(height)

for param in parameters:
    print(param)

matrix = np.random.rand(height, width)
print(matrix)
for row in range(height):
    for col in range(width):
        parameters.append(matrix[row][col])

# store in a csv
data = np.asarray(parameters)
#data.tofile('matrix_data.csv',sep=',',format='%10.5f')
for x in data:
    print(x)
data.tofile('matrix_data.csv',sep=',')