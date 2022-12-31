#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

typedef long Rate_t;

struct _valve {
    bool is_open;
    std::string name;
    Rate_t flow_rate;
    std::vector< _valve* > connections;
};
typedef _valve Valve_t;

Valve_t *Curr_Valve = NULL;
std::vector< Valve_t > All_Valves;
size_t Minute;

int makeNewValve(std::string valve_name) {
    std::string name_copy = valve_name;
    Valve_t new_valve = { false, name_copy, 0 };
    new_valve.connections.clear();
    All_Valves.push_back(new_valve);

    std::cout << std::endl << "--Made new valve named " << valve_name << " at (" 
              << &All_Valves.back() << ")" << std::endl << std::endl;
    
    return All_Valves.size() - 1;
}

int findValve(std::string valve_name) {
    int ret_val = -1;
    for (int i = 0; i < All_Valves.size(); i++) {
        if (All_Valves[i].name == valve_name) {
            ret_val = i;
            break;
        }
    }
    return ret_val;
}

void parseValves(std::ifstream *valve_file);
void printValve(Valve_t *v, bool show_connections);

//------------------------------------------------------------------------------

int main(int argc, char **argv) {
    if (argc <= 1 || argc > 2) {
        std::cout << "enter the filename while calling the program" << std::endl;
        return 1;
    }
    std::ifstream valve_data(argv[1], std::ios::in);
    if (!valve_data.is_open()) {
        std::cout << "cannot open " << argv[1] << std::endl;
        return 1;
    }

    parseValves(&valve_data);
    
    valve_data.close();
    return 0;
}

//------------------------------------------------------------------------------

void parseValves(std::ifstream *valve_file) {
    int valve_idx = 0, other_idx = 0;
    char c;
    Rate_t rate;
    std::string str, name;
    std::stringstream new_stream;
    while (std::getline(*valve_file, str)) {
        new_stream.clear();
        new_stream.str(str);

        new_stream >> str >> name;
        valve_idx = findValve(name);
        if (valve_idx == -1) {
            valve_idx = makeNewValve(name);
        }

        new_stream >> str >> str >> c >> c >> c >> c >> c >> rate >> c;
        All_Valves[valve_idx].flow_rate = rate;

        if (name == "AA") {
            Curr_Valve = &All_Valves[valve_idx];
        }

        std::cout << "Added details to " << All_Valves[valve_idx].name << " ("
                  << &All_Valves[valve_idx] << ") with rate " 
                  << All_Valves[valve_idx].flow_rate << std::endl;
        
        new_stream >> str >> str >> str >> str;
        while (new_stream >> name) {
            if (name.back() == ',') {
                name.pop_back();
            }
            other_idx = findValve(name);
            if (other_idx == -1) {
                other_idx = makeNewValve(name);
            }
            All_Valves[valve_idx].connections.push_back(&All_Valves[other_idx]);
            
            std::cout << "\tConnected with ";
            printValve(&All_Valves[other_idx], true);
            std::cout << std::endl;
        }
    }
}

void printValve(Valve_t *v, bool show_connections) {
    fprintf(stdout, "%s (0x%0x) - %2d (%s) ... ", v->name.c_str(), v, v->flow_rate, (v->is_open ? "OPEN" : "CLOSED"));
    if (v->connections.size() > 0) {
        std::cout << "Has " << v->connections.size() << " connections";
        if (show_connections) {
            std::cout << " :";
            for (Valve_t *_v : v->connections) {
                std::cout << " ";
                if (!_v) { std::cout << "-_"; }
                else { std::cout << _v->name; }
            }
        }
    }
    else {
        std::cout << "ISN'T DEFINED";
    }
}