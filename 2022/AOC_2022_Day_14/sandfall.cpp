#include "cave.hpp"

Cave_t Cave;
int Sand_Row, Sand_Col;
#define SOURCE_Col 500
#define SOURCE_Row 0

void noteRocks(std::stringstream *rocks) {
    int row, col;
    char ch;
    while (1) {
        *rocks >> col >> ch >> row;
        Cave.addObstacle(row, col, _Rock);
        if (*rocks >> ch >> ch) { continue; }
        else { break; }
    }
}

int main(int argc, char **argv) {
    if (argc <= 1 || argc > 2) {
        std::cout << "enter the filename while calling the program" << std::endl;
        return 1;
    }
    std::ifstream rocks_file(argv[1], std::ios::in);
    if (!rocks_file.is_open()) {
        std::cout << "cannot open " << argv[1] << std::endl;
        return 1;
    }

    std::stringstream rocks;
    std::string rock_line;
    while (std::getline(rocks_file, rock_line)) {
        rocks.str(rock_line);
        noteRocks(&rocks);
    }

    rocks_file.close();
    return 0;
}