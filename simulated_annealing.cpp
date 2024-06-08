#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>

// Define the car structure
struct Car {
    int id;
    std::vector<int> options;
};

// Define the problem constraints
const int NUM_CARS = 10;
const int NUM_OPTIONS = 3;
const int MAX_OPTIONS_PER_SEQUENCE = 2;
const double INITIAL_TEMPERATURE = 1000.0;
const double COOLING_RATE = 0.99;
const int MAX_ITERATIONS = 10000;

// Function to generate an initial random sequence
std::vector<Car> generateInitialSequence(std::vector<Car>& cars) {
    std::vector<Car> sequence = cars;
    std::random_shuffle(sequence.begin(), sequence.end());
    return sequence;
}

// Function to calculate the cost of a sequence
int calculateCost(const std::vector<Car>& sequence) {
    int cost = 0;
    std::vector<int> optionCounts(NUM_OPTIONS, 0);

    for (size_t i = 0; i < sequence.size(); ++i) {
        for (size_t j = 0; j < sequence[i].options.size(); ++j) {
            if (sequence[i].options[j]) {
                optionCounts[j]++;
                if (optionCounts[j] > MAX_OPTIONS_PER_SEQUENCE) {
                    cost++;
                }
            }
        }

        if ((i + 1) % MAX_OPTIONS_PER_SEQUENCE == 0) {
            std::fill(optionCounts.begin(), optionCounts.end(), 0);
        }
    }

    return cost;
}

// Function to swap two cars in the sequence
void swapCars(std::vector<Car>& sequence) {
    int idx1 = rand() % sequence.size();
    int idx2 = rand() % sequence.size();
    std::swap(sequence[idx1], sequence[idx2]);
}

// Simulated Annealing algorithm
std::vector<Car> simulatedAnnealing(std::vector<Car>& cars) {
    std::vector<Car> currentSequence = generateInitialSequence(cars);
    std::vector<Car> bestSequence = currentSequence;
    double temperature = INITIAL_TEMPERATURE;

    for (int iter = 0; iter < MAX_ITERATIONS; ++iter) {
        std::vector<Car> newSequence = currentSequence;
        swapCars(newSequence);

        int currentCost = calculateCost(currentSequence);
        int newCost = calculateCost(newSequence);

        if (newCost < currentCost || exp((currentCost - newCost) / temperature) > ((double) rand() / RAND_MAX)) {
            currentSequence = newSequence;
        }

        if (calculateCost(currentSequence) < calculateCost(bestSequence)) {
            bestSequence = currentSequence;
        }

        temperature *= COOLING_RATE;
    }

    return bestSequence;
}

// Main function
int main() {
    srand(time(NULL));

    // Define the cars with their options
    std::vector<Car> cars(NUM_CARS);
    for (int i = 0; i < NUM_CARS; ++i) {
        cars[i].id = i;
        cars[i].options = std::vector<int>(NUM_OPTIONS);
        for (int j = 0; j < NUM_OPTIONS; ++j) {
            cars[i].options[j] = rand() % 2;
        }
    }

    std::vector<Car> bestSequence = simulatedAnnealing(cars);

    // Output the best sequence
    std::cout << "Best sequence:" << std::endl;
    for (const Car& car : bestSequence) {
        std::cout << "Car " << car.id << ": ";
        for (int option : car.options) {
            std::cout << option << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
