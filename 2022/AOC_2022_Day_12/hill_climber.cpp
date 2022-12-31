#include "hill_climber.h"

Map_t Terrain;

void storeChar(char,int,int,std::vector<char>*);
void createMap(std::ifstream*);
void showMap();

void markLocation(int x,int y)    { Terrain.Map[y][x].is_marked = true;  }
void unmarkLocation(int x, int y) { Terrain.Map[y][x].is_marked = false; }

int findBestPath() {
    int total_steps = 0;

    // TODO


    return total_steps;
}

//------------------------------------------------------------------------------

int main(int argc, char **argv) {
    if (argc <= 1 || argc > 2) {
        std::cout << "enter the filename while calling the program" << std::endl;
        return 1;
    }
    std::ifstream hill_map(argv[1], std::ios::in);
    if (!hill_map.is_open()) {
        std::cout << "cannot open " << argv[1] << std::endl;
        return 1;
    }

    createMap(&hill_map);
    showMap();

    hill_map.close();
    return 0;
}

//------------------------------------------------------------------------------

void storeChar(char c, int x, int y, std::vector<Location_t> *buff) {
    switch (c) {
        case 'S':
            Terrain.start_x = x;
            Terrain.start_y = y;
            c = LOWEST_HEIGHT;
            break;
        case 'E':
            Terrain.end_x = x;
            Terrain.end_y = y;
            c = HIGHEST_HEIGHT;
            break;
        default:
            break;
    }
    Location_t l = {c, false};
    buff->push_back(l);
}

void createMap(std::ifstream *map_file) {
    std::string line_str;
    std::vector<Location_t> new_line;
    int y_depth = 0;
    while (std::getline(*map_file, line_str)) {
        new_line.clear();
        for (int x_depth = 0; x_depth < line_str.size(); x_depth++) {
            storeChar(line_str[x_depth], x_depth, y_depth, &new_line);
        }
        y_depth++;
        Terrain.Map.push_back(new_line);   
    }
}

void showMap() {    // TODO - Add Color
    char h;
    std::cout << "--- Terrain ---" << std::endl;
    for (int i = 0; i < Terrain.Map.size(); i++) {
        for (int j = 0; j < Terrain.Map[i].size(); j++) {
            if (i == Terrain.start_y && j == Terrain.start_x) {
                h = 'S';
            }
            else if (i == Terrain.end_y && j == Terrain.end_x) {
                h = 'E';
            }
            else {
                h = Terrain.Map[i][j].height;
            }
            std::cout << h;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}