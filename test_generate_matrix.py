# test script for reading the matrix csv file
# directly from https://realpython.com/python-csv/#reading-csv-files-with-csv
import csv
import numpy as np

with open('matrix_data.csv') as csv_file:
    csv_reader = csv.reader(csv_file, delimiter=',')
    data = list(csv_reader)
    data = data[0]
    line_count = 0
    for row in csv_reader:
        if line_count == 0:
            print(f'Column names are {", ".join(row)}')
            line_count += 1
        else:
            print(f'\t{row[0]} works in the {row[1]} department, and was born in {row[2]}.')
            line_count += 1
    print(f'Processed {line_count} lines.')


epsilon = float(data[0])
discount_factor = float(data[1])
learning_rate = float(data[2])
width = int(float(data[3]))
height = int(float(data[4]))

# printing hyper parameters
print("Epsilon:", epsilon)
print("Discount factor:", discount_factor)
print("Learning rate:", learning_rate)
print("Rows:", height)
print("Columns:", width)

matrix = np.zeros((height, width))

index = 5 # start at index 5 to skip parameters
for row in range(height):
    for col in range(width):
        matrix[row][col] = data[index]
        index += 1

print(matrix)