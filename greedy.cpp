#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>
#include <numeric>
#include <utility>
using namespace std;


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


// robada de stackoverflow, usar si es que la mia no funciona
/* vector<int> str_to_int(vector<string> input) {
    vector<std::string> input;
    vector<int> output;

    for (auto &s : input) {
        std::stringstream parser(s);
        int x = 0;

        parser >> x;

        output.push_back(x);
    }
    return output;
} */


// robada de stackoverflow
int rand_lb_ub(int lb, int ub) {
    // para un rand con up (upperbound) y lb (lowerbound)
    // rand() % (ub - lb + 1)) + lb
    return (rand() % (ub - lb + 1)) + lb;
}

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

problem instance; // se instancia aqui como variable "global"
// usaré esto ya que solo deberia haber una instancia por cada ejecucion
// esto es para no tener que insertarlo como parametro de todas las funciones

problem read_txt() {
    
    int num_cars, num_options, num_classes;
    vector<int> p;
    vector<int> q;
    vector<vector<int>> r;
    vector<int> d;
    
    ifstream file("data.txt");
    int i = 0;
    string str; 
    
    while (getline(file, str)) {   
        
        if (i == 0) {
            stringstream s(str);
            string num_cars_s, num_options_s, num_classes_s;
            s>>num_cars_s, num_options_s, num_classes_s;
            num_cars = stoi(num_cars_s);
            num_options = stoi(num_options_s);
            num_classes = stoi(num_classes_s);
            
        } else if (i == 1) {
            vector<string> pv = split(str, ' '); // capacidad relativa
            p = str_to_int(pv);
            
        } else if (i == 2) {
            vector<string> qv = split(str, ' '); // capacidad absoluta
            q = str_to_int(qv);
            
        } else {
            for (int i = 0; i < num_classes; i++) {
                vector<string> rv = split(str, ' ');
                d.push_back(stoi(rv[1])); // agregamos la demanda de cada clase al vector d
                rv.erase(rv.begin(),rv.begin() + 2); // eliminamos los primeros dos elementos que no son parte de la matriz4
                r.push_back(str_to_int(rv)); // agregamos la fila i a la matriz r
            }
        }
        i++;
    }
    
    instance = {num_cars, num_options, p, q, r, d};
    return instance;
}

vector<int> get_classes(vector<int> x, int option) {
    vector<int> x_bin (x.size(), 0);
    for (int i = 0; i < x.size(); i++) {
        if (instance.r[x[i]+1][option] == 1) {
            x_bin[i] = 1;
        }
    }
    return x_bin;
}

bool check_demand(vector<int> x) {
    for (int curr_class = 0; curr_class < instance.r.size(); curr_class++) {
        if (instance.d[curr_class] > accumulate(x.begin(), x.end(), 0)) {
            return false;
        }
    }
}

int eval(vector<int> x) {
    int sum = 0;
    for (int option = 0; option < instance.O; option++) {
        int curr_p = instance.p[option];
        int curr_q = instance.q[option];
        vector<int> x_bin = get_classes(x, option);
        for (int i = 0; i < x_bin.size(); i++) {
            vector<int> sub(&x_bin[i],&x_bin[i + curr_q]);
            sum = sum + accumulate(sub.begin(), sub.end(), 0) - curr_p;
        }
    }
    return sum;
}

vector<int> move(vector<int> x, int iteration) {
    // TODO cambiar a un swap
    int pos = iteration % x.size();
    pos--;  // esto para que la numero de iteracion corresponda al elemento del vector x
    int pos2;
    if (pos + 1 == x.size()) { // intercambia el primero y el ultimo en caso de que pos sea el ultimo
        pos2 = 0;
    }
    swap(x[pos],x[pos2]);
    return x;
}

vector<int> greedy(int iterations, problem instance, vector<int> x) {
    int iteration = 0;
    int curr_fitness = eval(x);
    for (int i = 0; i < iterations; i++) {
        vector<int> next_x = move(x, iteration);
        if (eval(next_x) < curr_fitness) { // realiza el movimiento si es que mejora la fitness
            x = next_x;
            curr_fitness = eval(x);
        }
        iteration++; // incrementa la iteracion si o si
    }
    return x;
}

int main() {
    return 0;
}

// para la generacion de instancias aleatorias, puede que no se use pero ya hice parte
// de la funcion asi que no la voy a desechar (no aun al menos)
/* tuple<int> random_gen(int seed) {
    srand(seed);
    
    int lower_cars = 8;
    int upper_cars = 12;
    
    int lower_options = 4;
    int upper_options = 6;
    
    int lower_classes = 5;
    int upper_classes = 7;
    
    int num_cars = rand_lb_ub(lower_cars, upper_cars);
    int num_options = rand_lb_ub(lower_options, upper_options);
    int num_classes = rand_lb_ub(lower_classes, upper_classes);
    
    
    int lower_p = 8;
    int upper_p = 12;
    
    int lower_q = 8;
    int upper_q = 12;
    
    for (int i = 0; i < num_options; i++) {
        for (int j = 0; i < num_options; j++) {
            
        }
    }
} */