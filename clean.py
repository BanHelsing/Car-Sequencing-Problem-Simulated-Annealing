import os

def process_file(file_path):
    with open(file_path, 'r') as file:
        lines = file.readlines()

    # Remove trailing whitespace from each line
    lines = [line.rstrip() for line in lines]

    # Remove empty lines at the end
    while lines and lines[-1] == '':
        lines.pop()

    # Write the processed lines back to the same file
    with open(file_path, 'w') as file:
        file.write('\n'.join(lines) + '\n')

# Example use
ls = os.listdir("data/medium/")
print(ls)
for file in ls:
    file_path = 'data/medium/' + file
    process_file(file_path)
