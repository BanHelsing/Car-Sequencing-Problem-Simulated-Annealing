#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <tuple>
#include <numeric>
#include <cmath>
#include <ctime>
#include <stdexcept>
#include <random>
#include <chrono>

using std::vector, std::cout, std::endl, std::string, std::tuple, std::get, std::to_string;
namespace fs = std::filesystem;

struct problem { // tupla definida por ROADEF
    int V;
    int O;
    vector<int> p;
    vector<int> q;
    vector<vector<int>> r;
    vector<int> d; // se agrega el vector d que contiene las demandas por clase
};

// inicio de sección read, todas estas funciones existen con el propósito de ayudar a read_txt

// robada de stackoverflow,  toma un string y lo separa segun el delimitador indicado, analogo al metodo split de python
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

// imprime un vector de manera ordenada, incluyendo su nombre
string print_vector(vector<int> vec, string name) {
    string whole = "";
    whole += name + ": {" + to_string(vec[0]);
    for (int i = 1; i < vec.size(); i++) {
        whole += ", " + to_string(vec[i]);
    }
    whole += "}, of size " + to_string(vec.size()) + "\n";
    cout << whole;
    return whole;
}

// inicializa el vector x con valores provenientes de d, así se cumple con la restriccion 2
vector<int> initial_solution(problem instance) {
    vector<int> x;
    int num_cars = instance.V;
    vector<int> d = instance.d;
    for (int i = 0; i < instance.d.size(); i++) {
        for (int j = 0; j < instance.d[i]; j++) {
            x.push_back(i);
        }
    }
    return x;
}

// imprime la tupla del problema, y lo retorna como string para ser usado despues
vector<string> print_problem(problem instance) {
    vector<string> whole;
    cout << "V: " << instance.V << "\n";
    whole.push_back("V: " + std::to_string(instance.V) + "\n");
    cout << "O: " << instance.O << "\n";
    whole.push_back("O: " + std::to_string(instance.O) + "\n");
    whole.push_back(print_vector(instance.p, "p"));
    whole.push_back(print_vector(instance.q, "q"));
    whole.push_back(print_vector(instance.d, "d"));
    for (int i = 0; i < instance.r.size(); i++) {
        whole.push_back(print_vector(instance.r[i], "r" + to_string(i)));
    }
    return whole;
}

// De acuerdo a lo extraido de CSPLib:
//    First line: number of cars; number of options; number of classes.
//    Second line: for each option, the maximum number of cars with that option in a block. (p)
//    Third line: for each option, the block size to which the maximum number refers. (q)
//    Then for each class: index no.; no. of cars in this class; for each option, whether or not this class requires it (1 or 0).
auto read_txt(string filename) {
    
    int num_cars, num_options, num_classes;
    vector<int> p;
    vector<int> q;
    vector<vector<int>> r;
    vector<int> d;
    
    std::ifstream file(filename);
    int i = 0;
    string str; 
    
    while (std::getline(file, str)) { // se lee el txt del problema linea por linea
        
        printf("Line number %d: %s\n", i, str.c_str());
        
        if (i == 0) { // primera linea
        
            std::stringstream s(str);
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
            
        } else if (i == 1) { // segunda linea
            
            vector<string> pv = split(str, ' '); // capacidad relativa
            p = str_to_int(pv);
            print_vector(p, "p");
            
        } else if (i == 2) { // tercera linea
            vector<string> qv = split(str, ' '); // capacidad absoluta
            q = str_to_int(qv);
            print_vector(q, "q");
            
            if (p.size() != q.size()) { // en caso de que haya incongruencia se retorna inmediatamente una instancia vacia y se indica el error
                printf("Error: p (%lu) and q (%lu) have different sizes\n\n", p.size(), q.size());
                problem instance = {0, num_options, p, q, r, d};
                return instance;
            } else {
                printf("p(%lu) and q (%lu) have the same size (that's good)\n\n", p.size(), q.size());
            }
            
        } else { // cuarta linea en adelante
            
            vector<string> rv = split(str, ' ');
            d.push_back(stoi(rv[1])); // agregamos la demanda de cada clase al vector d
            
            vector<string>(rv.begin() + 2, rv.end()).swap(rv);
            
            //rv.erase(rv.begin(),rv.begin() + 2); // eliminamos los primeros dos elementos que no son parte de la matriz4
            //print_vector(str_to_int(rv), "rv curr");
            
            r.push_back(str_to_int(rv)); // agregamos la fila i a la matriz r
        }
        i++;
    }
    
    if (r.size() != d.size()) {// en caso de que haya incongruencia se retorna inmediatamente una instancia vacia y se indica el error
        printf("Error: r (%lu) and d (%lu) have different sizes\n\n", r.size(), d.size());
        problem instance = {0, num_options, p, q, r, d};
        return instance;
    } else {
        printf("r(%lu) and d (%lu) have the same size (that's also good)\n\n", r.size(), d.size());
    }
    
    if (r.size() != num_classes) {// en caso de que haya incongruencia se retorna inmediatamente una instancia vacia y se indica el error
        printf("Error: num_classes (%i) and d (%lu) have different sizes\n\n", num_classes, d.size());
        problem instance = {0, num_options, p, q, r, d};
        return instance;
    } else {
        printf("num_classes(%i) and d (%lu) have the same size (that's also also good)\n\n", num_classes, d.size());
    }
    
    problem instance = {num_cars, num_options, p, q, r, d}; // se inicializa el problema con los datos extraidos del txt
    return instance;
}

// seccion de funciones base, estas sirven tanto para el algoritmo greedy como para el algoritmo simulated annealing

// revuelve el vector, se usó de manera experimental, pero no es parte de la implementación final
vector<int> shuffle_vector(vector<int> x) {
    int vector_size = x.size();
        for (int k = 0; k < x.size(); k++) {
        int r = k + rand() % (vector_size - k); // careful here!
        std::swap(x[k], x[r]);
    }
    return x;
}

// obtiene el vector x_bin, que es un vector binario indicando que vehiculos requieren la opcion "option"
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

// funcion de evaluacion, detallada en el informe
int eval(vector<int> x, problem instance) {
    int sum = 0;
    for (int option = 0; option < instance.O; option++) {
        int curr_p = instance.p[option];
        int curr_q = instance.q[option];
        vector<int> x_bin = get_classes(x, option, instance);
        
        for (int i = 0; i < x_bin.size() - curr_q; i++) {
            vector<int> sub = {x.begin() + i, x.begin() + i + curr_q}; 
            sum = sum + accumulate(sub.begin(), sub.end(), 0) - curr_p;
        }
    }
    return sum;
}

vector<int> move(vector<int> vec, int iteration) {
    int pos = iteration % vec.size();
    
    int temp1 = vec[pos]; // primero del par
    if (pos + 1 == vec.size()){ // si es que temp1 es el ultimo elemento del vector, temp2 será el primero
        int temp2 = vec[0]; 
        vec[pos] = temp2; // intercambio
        vec[0] = temp1;
    } else { // caso normal
        int temp2 = vec[pos + 1];
        vec[pos] = temp2;
        vec[pos + 1] = temp1;
    }
    return vec;
}

// funcion para crear los archivos log
void log_data(bool verbose, tuple<int, int, bool> *log, int log_size, string algorithm, string problemname, tuple<double, int, int, int, vector<int>, vector<string>> *data) {
    
    time_t now = time(nullptr);
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d_%H-%M-%S", localtime(&now)); // usado para el nombre, así no se repiten
    
    string timestamp_str = timestamp;
    
    if (verbose) {
        algorithm += "_verbose"; // en caso de querer info extra el inicio del txt
    }
    problemname = split(problemname, '/').back();
    cout << "Logging: " << timestamp_str << "_" << problemname << "_" << algorithm << ".txt" << endl;
    string filename = "../logs/log_" + timestamp_str + "_" + problemname + ".txt"; // crear el archivo
    std::ofstream log_file(filename);
    
    if (verbose) {
        log_file << "#time: "<< get<0>(*data) << "\n";
        log_file << "#iterations: "<< get<1>(*data) << "\n";
        log_file << "#algorithm: "<< algorithm << "\n";
        log_file << "#first fitness: "<< get<2>(*data) << "\n";
        log_file << "#final fitness: "<< get<3>(*data) << "\n";
        log_file << "#final solution: {" << get<4>(*data)[0];
        for (int i = 1; i < get<4>(*data).size(); i++) {
            log_file << "," << get<4>(*data)[i];
        }
        log_file << "}\n";
        log_file << "#problem\n";
        log_file << "#V: " << get<5>(*data)[0];
        log_file << "#O: " << get<5>(*data)[1];
        log_file << "#" << get<5>(*data)[2];
        log_file << "#" << get<5>(*data)[3];
        log_file << "#" << get<5>(*data)[4];
        for (int i = 5; i < get<5>(*data).size(); i++) {
            log_file << "#" << get<5>(*data)[i];
        }
    }
    
    log_file << "iteration,fitness,moved\n";
    
  // escribir en el archivo
    for (int i = 0; i < log_size; i++) {
        string s = std::to_string(get<0>(log[i])) + "," + std::to_string(get<1>(log[i])) + "," + std::to_string(get<2>(log[i]));
        log_file << s << "\n";
    }
    // cerrarlo
    log_file.close();
}

// algoritmo greedy
vector<int> greedy(int iterations, problem instance, vector<int> x, tuple<int, int, bool> *log) {
    vector<int> next_x;
    
    cout << "\nSolution at start of greedy: "; // solucion al inicio del greedy
    print_vector(x, "x");
    
    int curr_fitness = eval(x, instance); // obtiene primera fitness
    cout << "Evaluation at start of greedy: " << curr_fitness << endl;
    
    for (int i = 0; i < iterations; i++) { // for por las iteraciones
        next_x = move(x, i);
        if (eval(next_x, instance) < curr_fitness) { // realiza el movimiento si es que mejora la fitness
            x = next_x;
            curr_fitness = eval(x, instance); // actualiza la fitness actual
            log[i] = {i, curr_fitness, true}; // agrega al log
        } else {
            log[i] ={i, curr_fitness, false}; // agrega al log
        }
    }
    return x;
}

// funcion de probabilidad del algoritmo simulated annealing
bool test_prob(int curr_fitness, int next_fitness, float curr_temp) {
    // P(.) = e ^ (d_eval) / T)
    int delta = curr_fitness - next_fitness; // calcula delta
    if (delta == 0) {
        return false;
    }
    
    float prob = exp(delta / curr_temp);
    float rand_tmp = (float)(rand()) / (float)(RAND_MAX);
    
    if (rand_tmp < prob) {
        return true; // retorna verdadero si pasa la prueba
    } else {
        return false; // retorna falso si no pasa
    }
}

// algoritmo simulated annealing
vector<int> simulated_annealing(int iterations, problem instance, vector<int> x, float initial_temperature, unsigned reheats, float decay_factor, tuple<int, int, bool> *log) {
    vector<int> next_x;
    float curr_temp = initial_temperature;
    int curr_reheats = 0;
    cout << endl;
    cout << "Initial temperature: " << curr_temp << "\n";
    cout << "Decay factor: " << decay_factor << "\n";
    
    cout << "\nSolution at start of simulated annealing: ";
    print_vector(x, "x");
    
    int curr_fitness = eval(x, instance);
    int best_fitness = curr_fitness;
    vector<int> best_x = x;
    cout << "Evaluation at start of simulated annealing: " << curr_fitness << endl;

    for (int i = 0; i < iterations; i++) {
        next_x = move(x, i);
        int next_fitness = eval(next_x, instance);
        bool passed_temp = test_prob(curr_fitness, next_fitness, curr_temp);
        if (next_fitness < curr_fitness || passed_temp) { // realiza movimiento si es que pasa la prueba o mejora la fitness
            x = next_x;
            curr_fitness = eval(x, instance);
            log[i + instance.V*2] = std::make_tuple(i+instance.V*2, curr_fitness, true);
            if (curr_fitness < best_fitness) {
                best_fitness = curr_fitness;
                best_x = x;
            }
        } else { // si no hay movimiento se logea de todas formas
            log[i + instance.V*2] = std::make_tuple(i+instance.V*2, curr_fitness, false);
        }
        curr_temp *= decay_factor;
        if (curr_temp < 1 && curr_reheats < reheats) { // recalentamiento
            curr_temp = initial_temperature/(curr_reheats+1); // se divide por el recalentamiento actual, compensando con +1 ya que el primer recalentamiento es 0
            curr_reheats++;
        }
    }
    x = best_x;
    return x;
}

// para evaluar un solo problema, se usa cuando main evalua todos los problemas de una categoria
bool run(float initial_temperature, unsigned reheats, float decay_factor, int iterations, string algorithm_name, string filename) {
    cout << "Opening: " << filename << endl;
    problem instance = read_txt(filename);
    iterations *= instance.V;
    cout << iterations << endl;
    if (instance.V == 0) {
        return false;
    }
    
    vector<string> problem_string = print_problem(instance);
    vector<int> x = initial_solution(instance);
    x = shuffle_vector(x);
    tuple<int, int, bool> log[iterations +  instance.V * 2]; // (iteration, fitness, moved)
    cout << "log size: "<< sizeof(log)/sizeof(log[0]) << endl;
    tuple<int, int, bool> * log_ptr = &log[0]; // para pasar por referencia al array log, debido a que puede llegar a tener 500000 elementos de largo
    tuple<double, int, int, int, vector<int>, vector<string>> data;
    tuple<double, int, int, int, vector<int>, vector<string>> * data_ptr = &data; // igualmente con la data
    
    std::chrono::duration<double> greedy_time, sa_time;
    
    if (algorithm_name == "greedy") {
        auto greedy_start = std::chrono::high_resolution_clock::now(); // se inicia el cronómetro
        x = greedy(iterations, instance, x, log);
        auto greedy_end = std::chrono::high_resolution_clock::now(); // se detiene el cronómetro
        greedy_time = greedy_end - greedy_start; //se obtiene la diferencia de tiempos para obtener el tiempo de Greedy
        cout  << "greedy time: " << greedy_time.count() << "\n";
        log_data(true, log_ptr, iterations+instance.V*2, "greedy", filename, data_ptr);
        
    } else if (algorithm_name == "simulated annealing") {
        auto sa_start = std::chrono::high_resolution_clock::now(); //se inicializa el cronómetro
        x = simulated_annealing(iterations, instance, x, initial_temperature, reheats, decay_factor, log_ptr);
        auto sa_end = std::chrono::high_resolution_clock::now(); //se detiene el cronómetro
        sa_time = sa_end - sa_start; //se obtiene la diferencia de tiempos para obtener el tiempo de Greedy
        cout  << "simulated annealing time: " << sa_time.count() << "\n";
        log_data(true, log_ptr, iterations+instance.V*2, "simulated_annealing", filename, data_ptr);
        
    } else if (algorithm_name == "combined") { // si se quiere usar greedy y simulated annealing en secuencia
        int greedy_iterations = instance.V *2; // no hay diferencia entre 2 y 200
        
        int first_fitness = eval(x, instance);
        
        auto greedy_start = std::chrono::high_resolution_clock::now(); // inicio de timing
        x = greedy(greedy_iterations, instance, x, log);
        auto greedy_end = std::chrono::high_resolution_clock::now(); // fin de timing
        greedy_time = greedy_end - greedy_start; // la diferencia indica el tiempo de ejecucion
        
        int greedy_fitness = eval(x, instance);
        
        auto sa_start = std::chrono::high_resolution_clock::now(); // inicio de timing
        x = simulated_annealing(iterations, instance, x, initial_temperature, reheats, decay_factor, log_ptr);
        auto sa_end = std::chrono::high_resolution_clock::now(); // fin de timing
        sa_time = sa_end - sa_start; // la diferencia indica el tiempo de ejecucion
        
        int sa_fitness = eval(x, instance);
        
        cout << endl;
        cout << "Simulated annealing time: " << sa_time.count() << "\n";
        cout << "Greedy time:              " << greedy_time.count() << "\n";
        const double time_s = greedy_time.count() + sa_time.count();
        cout << "Combined time:            " << time_s << "\n";
        
        cout << endl;
        cout << "First fitness: " << first_fitness << "\n";
        cout << "Second fitness:" << greedy_fitness << "\n";
        cout << "Last fitness:  " << sa_fitness << "\n";
        const int final_fitness = sa_fitness;
        
        data = {time_s, iterations, first_fitness, final_fitness, x, problem_string};
        
        cout << "Improvement: " << abs(sa_fitness - first_fitness) << " units, " << abs((float(sa_fitness) / float(first_fitness) *100 ) - 100) << "\% better overall\n" << "\n";
        log_data(true, log_ptr, iterations+instance.V*2, "greedy+simulated_annealing", filename, data_ptr);
    }
    cout << endl;
    
    printf("Result after %d iterations using \"%s\"\n", iterations, algorithm_name.c_str());
    print_vector(x, "X");
    printf("Final fitness: %d\n\n", eval(x, instance));
    return true;
}

int main(int argc, const char **argv) {  // arg1 = algorithm name, arg2 = iterations count, arg3 = initial_temperature, arg4 = reheats, arg5 = decay_factor, arg6 = decay_function  
    
    //unsigned int seed = 14091321; // para testeo, en caso real se reemplaza con time(0) para obtener randomness "real"
    unsigned int seed = time(0);
    srand(seed);
    
    float initial_temperature;
    unsigned reheats;
    float decay_factor;
    string type;
    
    string algorithm_name = argv[1];
    if (algorithm_name == "sa") {
        algorithm_name = "simulated annealing"; // para usar "sa" en vez de "simulated annealing" como argumento de entrada
    }
    
    cout << "Algorithm: " << algorithm_name << endl;
    
    int iterations = std::stoi(argv[2]);
    if (argc > 4) { // los parametros entran por argv
        initial_temperature = std::stof(argv[3]);
        reheats = std::stoi(argv[4]);
        decay_factor = std::stof(argv[5]);
        type = argv[6];
    } else { // caso en el que se quiera usar solo el alhgorimo greedy
        type = argv[3];
        cout << "type: " << type << endl;
    }
    std::string path = "../data/small/"; // tamaño de las instancias, puede ser small o medium
    
    if (type == "all") { // si es que se quieren hacer todos los problemas de una categoria
        for (auto & entry : fs::directory_iterator(path)) {
            string filename = entry.path().string();
            if (!run(initial_temperature, reheats, decay_factor, iterations, algorithm_name, filename)) {
                continue;
            }
        }
    } else if (type == "single") { // si es que se quiere hacer solo uno, especificado abajo
        string filename = "../data/medium/pb_100_8_20_26.txt";
        if (!run(initial_temperature, reheats, decay_factor, iterations, algorithm_name, filename)) {
            return 0;
        }
    } else {
        cout << "Invalid type: " << type << endl;
        return 0;
    }
    return 0;
}