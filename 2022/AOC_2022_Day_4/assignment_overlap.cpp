#include <iostream>
#include <fstream>
#include <sstream>
#include <tuple>

typedef struct {
    int low;
    int high;
} Range_Tuple;

char findAssignmentOverlap(std::string pair_assignment) {
    Range_Tuple elf1 = {0, 0}, elf2 = {0, 0};
    char temp_char;     // to read the characters seperating the numbers in the string
    std::stringstream str_stream(pair_assignment);
    str_stream >> elf1.low >> temp_char >> elf1.high >> temp_char >> elf2.low >> temp_char >> elf2.high;

    if ((elf1.high < elf2.low) || (elf2.high < elf1.low)) { // no overlap
        temp_char = 'n';
    }
    else if ((elf1.low > elf2.low) && (elf1.high > elf2.high)) {    // configuration for partial overlap
        temp_char = 'p';
    }
    else if ((elf1.low < elf2.low) && (elf1.high < elf2.high)) {    // configuration for partial overlap
        temp_char = 'p';
    }
    else {  // all other configurations have full overlaps
        temp_char = 'f';
    }
    // printf("%d-%d,%d-%d ==> %c\n", elf1.low, elf1.high, elf2.low, elf2.high, temp_char);
    return temp_char;
}

std::tuple<int, int> findOverlaps(std::ifstream *file) {
    std::tuple<int, int> num_overlaps = {0, 0}; // {partial_overlaps, full_overlaps}
    std::string raw_assignment_data;
    while (*file >> raw_assignment_data) {
        switch (findAssignmentOverlap(raw_assignment_data)) {
            case 'p':   // partial overlap
                std::get<0>(num_overlaps)++;
                break;
            case 'f':   // full overlap
                std::get<1>(num_overlaps)++;
                break;
            default: break; // no overlap
        }
    }
    return num_overlaps;
}

int main(int argc, char **argv) {
    if (argc <= 1 || argc > 2) {
        std::cout << "enter the filename while calling the program" << std::endl;
        return 1;
    }
    std::ifstream assignment_data;
    assignment_data.open(argv[1], std::ios::in);
    if (!assignment_data.is_open()) {
        std::cout << "cannot open file" << std::endl;
        return 1;
    }
    std::tuple<int, int> overlaps_count = findOverlaps(&assignment_data);
    int num_p = std::get<0>(overlaps_count), num_f = std::get<1>(overlaps_count);
    std::cout << "Full overlaps: "   << num_f         << std::endl
              << "Parial overlaps: " << num_p         << std::endl
              << "Total overlaps: "  << num_p + num_f << std::endl;
    return 0;
}