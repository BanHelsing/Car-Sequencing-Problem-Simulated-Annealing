import pandas as pd
import matplotlib.pyplot as plt
import os

# Read the logged data
ls = os.listdir("logs/")
print(ls)

file1 = ls[-1]
file2 = ls[-2]
print(file1, file2)

df1 = pd.read_csv("logs/" + file1) # simulated annealing
df2 = pd.read_csv("logs/" + file2) # greedy

# Find the maximum iteration value in df1
max_iteration = df2['iteration'].max()

# Adjust the iteration values in df2
df1['iteration'] += max_iteration

# Append df2 to df1
df = pd.concat([df2, df1], ignore_index=True)


# Display the first few rows of the dataframe


# Perform analysis or plotting

plt.plot(df['iteration'], df['fitness'])
plt.xlabel('Iteration')
plt.ylabel('Fitness')
plt.title('Fitness Over Iterations for Simulated Annealing Algorithm')
plt.show()