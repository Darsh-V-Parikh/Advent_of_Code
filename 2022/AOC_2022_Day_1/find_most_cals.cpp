#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
using namespace std;

vector< tuple<int, int> > elfCalories;

void sort_new_guy() {
    int i = elfCalories.size() - 1;
    while (i > 0 && (get<1>(elfCalories[i]) > get<1>(elfCalories[i-1]))) {
        tuple<int, int> tmp = elfCalories[i];
        elfCalories[i] = elfCalories[i-1];
        elfCalories[i-1] = tmp;
        i--;
    }
}

void extract_calorie_data(ifstream *file) {
    int elf_idx = 0, elf_cals = 0;
    string curr_cal;
    while (getline(*file, curr_cal)) {
        if (curr_cal.size() == 0) {
            tuple<int, int> new_elf = {elf_idx++, elf_cals};
            elfCalories.push_back(new_elf);
            sort_new_guy();
            elf_cals = 0;
        }
        else {
            elf_cals += stoi(curr_cal);
        }
    }
}

void calorie_info(int num_elves) {
    int total = 0;
    for (int i = 0; i < num_elves; i++) {
        cout << "Elf " << get<0>(elfCalories[i]) << " has " << get<1>(elfCalories[i]) << " calories" << endl;
        total += get<1>(elfCalories[i]);
    }
    cout << endl << "Total Calories: " << total << endl;
}

int main(int argc, char **argv) {
    ifstream fstream;
    fstream.open("input.txt", ios::in);
    extract_calorie_data(&fstream);
    if ((argc > 1) && (argc < 2) && (atoi(argv[1]) < (int)elfCalories.size())) {
        calorie_info((int)elfCalories.size());
    }
    else {
        calorie_info(3);
    }
    return 0;
}