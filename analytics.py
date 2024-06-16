import pandas as pd
import matplotlib.pyplot as plt
import os

# Read the logged data
ls = os.listdir("logs/")
print(ls)
greedy_list = []
simulated_annealing_list = []

ls = ls[-2:]

for file in ls:
    f = file.split("_")
    algorithm = f[-1]
    if algorithm == "greedy.csv":
        greedy_list.append(file)
    elif algorithm == "annealing.csv":
        simulated_annealing_list.append(file)

df_greedy = pd.read_csv("logs/" + greedy_list[0])
df_simulated_annealing = pd.read_csv("logs/" + simulated_annealing_list[0])

# Display the first few rows of the dataframe


# Perform analysis or plotting

plt.plot(df_greedy['iteration'], df_greedy['fitness'])
plt.xlabel('Iteration')
plt.ylabel('Fitness')
plt.title('Fitness Over Iterations for Greedy Algorithm')
plt.show()

plt.plot(df_simulated_annealing['iteration'], df_simulated_annealing['fitness'])
plt.xlabel('Iteration')
plt.ylabel('Fitness')
plt.title('Fitness Over Iterations for Simulated Annealing Algorithm')
plt.show()