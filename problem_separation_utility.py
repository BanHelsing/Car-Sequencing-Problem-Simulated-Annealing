import re
import os

def split_problems(input_file, output_dir):
    """
    Splits a text file containing car problems into separate files,
    each holding the numerical data for one problem.

    Args:
        input_file (str): The path to the text file containing car problems.
        output_dir (str): The path to the folder where the output files will be saved. Defaults to 'problems'.
    """
    # Create the output directory if it doesn't exist
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    with open(input_file, 'r') as file:
        content = file.read()

    # Regular expression to match each problem block
    problems = re.split(r'#[-]+', content)
    problem_number = 1

    for problem in problems:
        if problem.strip():  # Check if the problem block is not empty
            # Extract numerical data
            numerical_data = re.findall(r'^\d+.*', problem, re.MULTILINE)
            if numerical_data:
                # Create output file name
                output_file = os.path.join(output_dir, f'problem_{problem_number}.txt')
                with open(output_file, 'w') as out_file:
                    out_file.write('\n'.join(numerical_data))
                problem_number += 1

#input_file = os.path.join('data', 'car_problems.txt')  # Assuming data folder in same directory
#output_dir = 'problems'  # Change this to your desired output directory name
#split_problems(input_file, output_dir)


