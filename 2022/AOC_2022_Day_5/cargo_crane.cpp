#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
using namespace std;

typedef char stack_data_t;
typedef vector<stack_data_t> stack_t;
void display_stacks(int, stack_t[]);

tuple<stack_t*, stack_t*> extractStacks(ifstream *data_file, int* stack_size) {
    string temp_line;                          // string to store the first line
    do {                                            // loop to read the first line and note it's size
        temp_line.push_back((*data_file).get());
    } while (temp_line.back() != '\n');
    (*stack_size) = (temp_line.size() + 1) / 4;     // formula to find the number of stacks from line size
    while (temp_line.size() > 0) {                  // push back that first line, "rewind" the stream
        (*data_file).putback(temp_line.back());
        temp_line.pop_back();
    }

    stack_t *stack_array_9000 = new stack_t[*stack_size];
    stack_t *stack_array_9001 = new stack_t[*stack_size];

    while (true) {
        temp_line.clear();
        getline(*data_file, temp_line);
        if (temp_line.size() == 0) { break; }       // stop reading if line is empty
        for (int i = 0; i < *stack_size; i++) {     // check each stack location
            if (temp_line[(4*i)+1] != ' ') {        // does line have a character at that stack location
                stack_array_9000[i].insert(stack_array_9000[i].begin(), temp_line[(4*i)+1]);
            }
        }
    }
    for (int i = 0; i < *stack_size; i++) {                         // while loop accidently read the stack numbers line
        stack_array_9000[i].erase(stack_array_9000[i].begin());     // so remove 1 element from each stack
        stack_array_9001[i] = stack_array_9000[i];                  // copy stack for the 2nd part of challenge
    }

    tuple<stack_t*, stack_t*> stacks = {stack_array_9000, stack_array_9001};
    return stacks;
}

void moveStacks(int stack_size, tuple<stack_t*, stack_t*> *stacks, ifstream *instructions) {
    string temp_str;
    int src, dst, amnt;

    while (*instructions >> temp_str >> amnt >> temp_str >> src >> temp_str >> dst) {
        for (int i = 0; i < amnt; i++) {
            // CrateMover 9000
            get<0>(*stacks)[dst-1].push_back(get<0>(*stacks)[src-1].back());
            get<0>(*stacks)[src-1].pop_back();

            // CrateMover 9001
            get<1>(*stacks)[dst-1].push_back(get<1>(*stacks)[src-1][get<1>(*stacks)[src-1].size()-amnt+i]);
        }
        while (amnt--) {
            get<1>(*stacks)[src-1].pop_back();
        }
    }

    return;
}

void stackResult(int stack_size, tuple<stack_t*, stack_t*> *stacks) {
    cout << "CrateMove 9000: ";
    for (int i = 0; i < stack_size; i++) {
        cout << get<0>(*stacks)[i].back();
    }
    cout << endl << "CrateMover 9001: ";
    for (int i = 0; i < stack_size; i++) {
        cout << get<1>(*stacks)[i].back();
    }
    cout << endl;
}


int main(int argc, char **argv) {
    if (argc <= 1 || argc > 2) {
        cout << "enter the filename while calling the program" << endl;
        return 1;
    }
    ifstream stacks_data(argv[1], ios::in);
    if (!stacks_data.is_open()) {
        cout << "cannot open " << argv[1] << endl;
        return 1;
    }
    int num_stacks;

    tuple<stack_t*, stack_t*> stacks = extractStacks(&stacks_data, &num_stacks);
    moveStacks(num_stacks, &stacks, &stacks_data);
    stackResult(num_stacks, &stacks);
    delete get<0>(stacks);
    delete get<1>(stacks);
    stacks_data.close();
    return 0;
}