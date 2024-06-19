import pandas as pd
import matplotlib.pyplot as plt
import os

# Read the logged data
ls = os.listdir("logs/")
print(ls)

file = ls[-1]

# Append df2 to df1
df = pd.read_csv("logs/" + file, skiprows=15)


# Display the first few rows of the dataframe


# Perform analysis or plotting

plt.plot(df['iteration'], df['fitness'])
plt.xlabel('Iteration')
plt.ylabel('Fitness')
plt.title('Fitness Over Iterations for Simulated Annealing Algorithm')
plt.show()