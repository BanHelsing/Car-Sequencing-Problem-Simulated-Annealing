#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>
#include <numeric>
#include <cmath>
#include <ctime>
#include <stdexcept>
using namespace std;


// De acuerdo a lo extraido de CSPLib:
//    First line: number of cars; number of options; number of classes.
//    Second line: for each option, the maximum number of cars with that option in a block. (p)
//    Third line: for each option, the block size to which the maximum number refers. (q)
//    Then for each class: index no.; no. of cars in this class; for each option, whether or not this class requires it (1 or 0).

struct problem { // tupla definida por ROADEF
    int V;
    int O;
    vector<int> p;
    vector<int> q;
    vector<vector<int>> r;
    vector<int> d; // se agrega el vector d que contiene las demandas por clase
};


// inicio de sección read, todas estas funciones existen con el propósito de ayudar a read_txt

// robada de stackoverflow
vector<string> split(const string &text, char sep) {
    vector<string> tokens;
    size_t start = 0, end = 0;
    while ((end = text.find(sep, start)) != string::npos) {
        tokens.push_back(text.substr(start, end - start));
        start = end + 1;
    }
    tokens.push_back(text.substr(start));
    return tokens;
}

// pasa un vector de strings a un vector de ints
vector<int> str_to_int(vector<string> input) {
    vector<int> output (input.size(), 0);
    for (int i = 0; i < input.size(); i++) {
        output[i] = stoi(input[i]);
    }
    return output;
}   

void print_vector(vector<int> vec, string name) {
    cout << name << ": {" << vec[0];
    for (int i = 1; i < vec.size(); i++) {
        cout << ", " << vec[i];
    }
    cout << "}, of size " << vec.size() << endl;
}

// robada de stackoverflow
int rand_lb_ub(int lb, int ub) {
    // para un rand con up (upperbound) y lb (lowerbound)
    // rand() % (ub - lb + 1)) + lb
    return (rand() % (ub - lb + 1)) + lb;
}

vector<int> initial_solution(problem instance) {
    vector<int> x;
    int num_cars = instance.V;
    vector<int> d = instance.d;
    for (int i = 0; i < instance.d.size(); i++) {
        for (int j = 0; j < instance.d[i]; j++) {
            x.push_back(i);
        }
    }
    if (x.size() == num_cars) {
        printf("Correct demand\n");
    }
    return x;
}

void print_problem(problem instance) {
    cout << "V: " << instance.V << "\n";
    cout << "O: " << instance.O << "\n";
    print_vector(instance.p, "p");
    print_vector(instance.p, "q");
    print_vector(instance.d, "d");
    for (int i = 0; i < instance.r.size(); i++) {
        print_vector(instance.r[i], "r");
    }
}

problem read_txt(string filename) {
    
    int num_cars, num_options, num_classes;
    vector<int> p;
    vector<int> q;
    vector<vector<int>> r;
    vector<int> d;
    
    ifstream file(filename);
    int i = 0;
    string str; 
    
    while (getline(file, str)) {   
        
        printf("Line number %d: %s\n", i, str.c_str());
        
        if (i == 0) {
            stringstream s(str);
            string num_cars_s, num_options_s, num_classes_s;
            
            s>> num_cars_s;
            num_cars = stoi(num_cars_s);
            s>> num_options_s;
            num_options = stoi(num_options_s);
            s>> num_classes_s;
            num_classes = stoi(num_classes_s);
            
            printf("Num cars: %s\n", num_cars_s.c_str());
            printf("Num options: %s\n", num_options_s.c_str());
            printf("Num classes: %s\n\n", num_classes_s.c_str());
            
        } else if (i == 1) {
            
            vector<string> pv = split(str, ' '); // capacidad relativa
            p = str_to_int(pv);
            print_vector(p, "p");
            
        } else if (i == 2) {
            vector<string> qv = split(str, ' '); // capacidad absoluta
            q = str_to_int(qv);
            print_vector(q, "q");
            
            if (p.size() != q.size()) {
                printf("Error: p (%lu) and q (%lu) have different sizes\n\n", p.size(), q.size());
                throw out_of_range("mismatch!");
            } else {
                printf("p(%lu) and q (%lu) have the same size (that's good)\n\n", p.size(), q.size());
            }
            
        } else {
            
            vector<string> rv = split(str, ' ');
            d.push_back(stoi(rv[1])); // agregamos la demanda de cada clase al vector d
            
            vector<string>(rv.begin() + 2, rv.end()).swap(rv);
            
            //rv.erase(rv.begin(),rv.begin() + 2); // eliminamos los primeros dos elementos que no son parte de la matriz4
            //print_vector(str_to_int(rv), "rv curr");
            
            r.push_back(str_to_int(rv)); // agregamos la fila i a la matriz r
        }
        i++;
    }
    
    if (r.size() != d.size()) {
        printf("Error: r (%lu) and d (%lu) have different sizes\n\n", r.size(), d.size());
        throw out_of_range("mismatch!");
    } else {
        printf("r(%lu) and d (%lu) have the same size (that's also good)\n\n", r.size(), d.size());
    }
    
    if (r.size() != num_classes) {
        printf("Error: num_classes (%i) and d (%lu) have different sizes\n\n", num_classes, d.size());
        throw out_of_range("mismatch!");
    } else {
        printf("num_classes(%i) and d (%lu) have the same size (that's also also good)\n\n", num_classes, d.size());
    }
    
    problem instance = {num_cars, num_options, p, q, r, d};
    return instance;
}

// seccion de funciones base, estas sirven tanto para el algoritmo greedy como para el algoritmo simulated annealing

vector<int> get_classes(vector<int> x, int option, problem instance) {
    vector<int> x_bin (x.size(), 0);
    int curr_class;
    
    for (int i = 0; i < x.size(); i++) {
        curr_class = x[i];
        if (instance.r[curr_class][option] == 1) {
            x_bin[i] = 1;
        }
    }
    return x_bin;
}

bool check_demand(vector<int> x, problem instance) {
    for (int curr_class = 0; curr_class < instance.r.size(); curr_class++) {
        if (instance.d[curr_class] > accumulate(x.begin(), x.end(), 0)) {
            return false;
        }
    }
    return true;
}

int eval(vector<int> x, problem instance) {
    int sum = 0;
    for (int option = 0; option < instance.O; option++) {
        int curr_p = instance.p[option];
        int curr_q = instance.q[option];
        vector<int> x_bin = get_classes(x, option, instance);
        //cout << endl;
        //print_vector(x_bin, "x_bin");
        for (int i = 0; i < x_bin.size() - curr_q; i++) {
            //cout << "current violations: " << sum << endl;
            vector<int> sub = {x.begin() + i, x.begin() + i + curr_q}; 
            sum = sum + accumulate(sub.begin(), sub.end(), 0) - curr_p;
        }
    }
    //cout << "Evaluation: " << sum << endl;
    return sum;
}

vector<int> move(vector<int> vec, int iteration) {
    
    int pos = iteration % vec.size();
    
    //cout << "iteration: " << iteration << endl;
    //cout << "pos: " << pos << endl;
    //pos--;  // esto para que la numero de iteracion corresponda al elemento del vector x
    //? supongo que no es necesario? partimos desde la iteracion 0 nomas yera
    
    int temp1 = vec[pos]; // primero del par
    if (pos + 1 == vec.size()){
        int temp2 = vec[0]; // si es que temp1 es el ultimo elemento del vector, temp2 será el primero
        vec[pos] = temp2;
        vec[0] = temp1;
    } else {
        int temp2 = vec[pos + 1];
        vec[pos] = temp2;
        vec[pos +1 ] = temp1;
    }
    return vec;
}

void log_data(tuple<int, int, bool> *log, int iterations, string algorithm) {
    
    time_t now = time(nullptr);
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d_%H-%M-%S", localtime(&now));

    string timestamp_str = timestamp;
    
    string filename = "../logs/log_" + timestamp_str + "_" + algorithm + ".csv";
    ofstream log_file(filename);
    
    log_file << "iteration,fitness,moved\n";
    
  // Write to the file
    for (int i = 0; i < iterations; i++) {
        string s = to_string(get<0>(log[i])) + "," + to_string(get<1>(log[i])) + "," + to_string(get<2>(log[i]));
        log_file << s << "\n";
    }
    // Close the file
    log_file.close();
}

vector<int> greedy(int iterations, problem instance, vector<int> x) {    
    vector<int> next_x;
    
    tuple<int, int, bool> log[iterations]; // (iteration, fitness, moved)
    
    cout << "\nFirst solution: ";
    print_vector(x, "x");
    cout << endl;
    
    int curr_fitness = eval(x, instance);
    cout << "First evaluation: " << curr_fitness << endl;
    
    for (int i = 0; i < iterations; i++) {
        next_x = move(x, i);
        //print_vector(x, "curr_x");
        //print_vector(next_x, "next_x");
        //cout << "iteration: " << iteration << endl;
        if (eval(next_x, instance) < curr_fitness) { // realiza el movimiento si es que mejora la fitness
            x = next_x;
            curr_fitness = eval(x, instance);
            log[i] = make_tuple(i, curr_fitness, true);
        } else {
            log[i] = make_tuple(i, curr_fitness, false);
        }
    }
    log_data(log, iterations, "greedy");
    return x;
}

bool test_prob(int curr_fitness, int next_fitness, float curr_temp) {
    // P(.) = e ^ (d_eval) / T)
    int delta = curr_fitness - next_fitness;
    int prob = exp(delta / curr_temp) * 100;
    int rand_tmp = rand() % 100 + 1;
    if (rand_tmp < prob) {
        return true;
    } else {
        return false;
    }
}

vector<int> simulated_annealing(int iterations, problem instance, vector<int> x, int initial_temperature, float decay_factor) {
    vector<int> next_x;
    float curr_temp = initial_temperature;
    
    cout << "\nFirst solution: ";
    print_vector(x, "x");
    cout << endl;
    
    tuple<int, int, bool> log[iterations]; // (iteration, fitness, moved)
    
    int curr_fitness = eval(x, instance);
    cout << "First evaluation: " << curr_fitness << endl;
    
    for (int i = 0; i < iterations; i++) {
        next_x = move(x, i);
        //print_vector(x, "curr_x");
        //print_vector(next_x, "next_x");
        //cout << "iteration: " << iteration << endl;
        int next_fitness = eval(next_x, instance);
        if (next_fitness < curr_fitness || test_prob(curr_fitness, next_fitness, curr_temp)) {
            x = next_x;
            curr_fitness = eval(x, instance);
            log[i] = make_tuple(i, curr_fitness, true);
        } else {
            log[i] = make_tuple(i, curr_fitness, false);
        }
        curr_temp = curr_temp * decay_factor;
    }
    log_data(log, iterations, "simulated_annealing");
    return x;
}

int main(int argc, const char * argv[]) { // arg1 = algorithm name, arg2 = iterations count, arg3 = initial_temperature, arg4 = decay_factor
    string filename = "../data.txt";
    
    problem instance = read_txt(filename);
    
    int initial_temperature;
    int decay_factor;
    
    string algorithm_name = argv[1];
    if (algorithm_name == "sa") {
        algorithm_name = "simulated annealing"; // para usar sa en vez de simulated annealing como argumento de entrada
    }
    
    cout << "Algorithm: " << algorithm_name << endl;
    
    int iterations = stoi(argv[2]);
    if (argc > 3) {
        initial_temperature = stoi(argv[3]);
        decay_factor = stoi(argv[4]);
    }
    
    unsigned int seed = 14091321; // para testeo, en caso real podria reemplazarse con time(0)
    srand(seed);
    
    print_problem(instance);
    vector<int> x = initial_solution(instance);
    
    if (algorithm_name == "greedy") {
        x = greedy(iterations, instance, x);
    } else if (algorithm_name == "simulated annealing") {
        x = simulated_annealing(iterations, instance, x, initial_temperature, decay_factor);
    }
    cout << endl;
    
    printf("Result after %d iterations using \"%s\"\n", iterations, algorithm_name.c_str());
    print_vector(x, "X");
    printf("Final fitness: %d\n", eval(x, instance));
    return 0;
}