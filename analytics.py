import pandas as pd
import matplotlib.pyplot as plt
import os

# Read the logged data
ls = os.listdir("logs/")

file = ls[-1]

def find_first_non_commented_line(file_path):
    with open(file_path, 'r') as file:
        for line_number, line in enumerate(file, start=1):
            stripped_line = line.strip()
            if not stripped_line.startswith('#'):
                return line_number
    return None

# Append df2 to df1
file_path = "logs/" + file
df = pd.read_csv(file_path, skiprows=find_first_non_commented_line(file_path)-1)

# Perform analysis or plotting
plt.plot(df['iteration'], df['fitness'])
plt.xlabel('Iteration')
plt.ylabel('Fitness')
plt.title('Fitness Over Iterations for Simulated Annealing Algorithm')
plt.show()