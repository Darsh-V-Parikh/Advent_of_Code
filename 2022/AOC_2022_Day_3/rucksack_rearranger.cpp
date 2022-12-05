#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
using namespace std;

vector<int> rucksacks;

int get_priority(char element) {
    if (element > 90) {    // Lower case
        return (element - 'a' + 1);
    }
    else {      // Upper Case
        return (element - 'A' + 27);
    }
}

// divide input string into half, then find which char is common across both halves
int sack_priority1(string sack_contents) {
    int divider_idx = sack_contents.size() / 2;
    for (int i = 0; i < divider_idx; i++) {
        for (int j = divider_idx; j < sack_contents.size(); j++) {
            if (sack_contents[i] == sack_contents[j]) {
                return get_priority(sack_contents[i]);
            }
        }
    }
    return 0;   // no mistake in organizing => no priority
}

// find which character is common across all strings
int sack_priority2(string sack1, string sack2, string sack3) {
    for (int i = 0; i < sack1.size(); i++) {
        for (int j = 0; j < sack2.size(); j++) {
            if (sack1[i] == sack2[j]) {
                for (int k = 0; k < sack3.size(); k++) {
                    if (sack3[k] == sack1[i]) {
                        return get_priority(sack1[i]);
                    }
                }
            }
        }
    }
    return 0;   // no mistake in organizing => no priority
}

void prioratize_sacks(ifstream *file, int sharing_type) {
    int sack_idx = 0;
    if (sharing_type == 1) {
        string curr_sack;
        while (*file >> curr_sack) {
            rucksacks.push_back(sack_priority1(curr_sack));
        }
    }
    else if (sharing_type == 2) {
        string s1, s2, s3;
        while (*file >> s1 >> s2 >> s3) {
            rucksacks.push_back(sack_priority2(s1, s2, s3));
        }
    }
}

void sack_info() {
    int total = 0;
    for (int i = 0; i < rucksacks.size(); i++) {
        total += rucksacks[i];
    }
    cout << "Total Priority: " << total << endl;
}

int main(int argc, char **argv) {
    if (argc <= 1 || argc > 2) {
        cout << "enter the filename while calling the program" << endl;
        return 1;
    }
    ifstream sack_data;
    sack_data.open(argv[1], ios::in);
    if (!sack_data.is_open()) {
        cout << "cannot open file" << endl;
        return 1;
    }
    prioratize_sacks(&sack_data, 2);    // 1 for 1st part of challenge, 2 for 2nd
    sack_info();
    return 0;
}