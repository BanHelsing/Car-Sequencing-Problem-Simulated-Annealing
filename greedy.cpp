#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
using namespace std;

// First line: number of cars; number of options; number of classes.
// Second line: for each option, the maximum number of cars with that option in a block.
// Third line: for each option, the block size to which the maximum number refers.
// Then for each class: index no.; no. of cars in this class; for each option, whether or not this class requires it (1 or 0).

void read_txt() { }

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

vector<int> vector_str_to_int(vector<string> v) {
    //TODO
}

int main() {
    ifstream file("data.txt");
    int i = 0;
    string str; 
    while (getline(file, str)) {   
        if (i == 0) {
            string num_cars, num_options, num_classes;
            stringstream s(str);
            s>>num_cars>>num_options>>num_classes;
            num_cars = stoi(num_cars);
            num_options = stoi(num_options);
            num_classes = stoi(num_classes);
        } else if (i == 1) {
            vector<string> p = split(str, ' '); // capacidad relativa
        } else if (i == 2) {
            vector<string> q = split(str, ' '); // capacidad absoluta
        } else {
            
        }
        i++;
    }
}