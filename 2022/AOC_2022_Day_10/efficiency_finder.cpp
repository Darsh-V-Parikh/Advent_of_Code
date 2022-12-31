#include <iostream>
#include <fstream>
#include <vector>

#define CRT_WIDTH    40
#define CRT_HEIGHT   6
#define SPRITE_WIDTH 3

int Cycle = 1, X = 1;
std::vector<int> SignalStrengths;
bool CRT[CRT_HEIGHT][CRT_WIDTH];
int CurrCRT_X = 0, CurrCRT_Y = 0;

void clearCRT() {
    for (int i = 0; i < CRT_HEIGHT; i++) {
        for (int j = 0; j < CRT_WIDTH; j++) {
            CRT[i][j] = false;
        }
    }
}

void drawOnCRT() {
    static int sprite_len = SPRITE_WIDTH / 2;  // how much of a sprite is on each side of the center
    CRT[CurrCRT_Y][CurrCRT_X] = ((CurrCRT_X >= X-sprite_len) && (CurrCRT_X <= X+sprite_len));
    if (++CurrCRT_X >= CRT_WIDTH) {
        CurrCRT_X = 0;
        if (++CurrCRT_Y >= CRT_HEIGHT) {
            CurrCRT_Y = 0;
        }
    }
}

void incrementCycle() {
    // fprintf(stdout, "\tX during Cycle %4d = %4d\n", Cycle, X);
    drawOnCRT();
    SignalStrengths.push_back(Cycle * X);
    Cycle++;
}

void addInstruction(int val) {
    incrementCycle();
    incrementCycle();
    X += val;
}

void noOperation() {
    incrementCycle();
}

void instructionDecoder(std::ifstream *code) {
    std::string instr, value;
    while (*code >> instr) {
        if (instr == "addx") {
            *code >> value;
            addInstruction(atoi(value.c_str()));
        }
        else if (instr == "noop") {
            noOperation();
        }
        // more in case of more instructions
    
        // std::cout << "\t -- -- -- -- -- -- --" << std::endl;
    }
}

void getCycleData() {
    int sum = 0;
    int wanted[] = {20, 60, 100, 140, 180, 220};
    for (int i : wanted) {
        fprintf(stdout, "Signal @ Cycle %4d = %d\n", i, SignalStrengths[i-1]);
        sum += SignalStrengths[i-1];
    }
    std::cout << std::endl << "Total Strength : " << sum << std::endl;
}

void displayCRT() {
    std::cout << std::endl << "Current State of CRT:" << std::endl;
    for (int i = 0; i < CRT_HEIGHT; i++) {
        fprintf(stdout, "\t");
        for (int j = 0; j < CRT_WIDTH; j++) {
            std::cout << (CRT[i][j] ? '#' : '.');
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}


int main(int argc, char **argv) {
    if (argc <= 1 || argc > 2) {
        std::cout << "enter the filename while calling the program" << std::endl;
        return 1;
    }
    std::ifstream cpu_code(argv[1], std::ios::in);
    if (!cpu_code.is_open()) {
        std::cout << "cannot open " << argv[1] << std::endl;
        return 1;
    }

    clearCRT();
    instructionDecoder(&cpu_code);
    getCycleData();
    displayCRT();

    cpu_code.close();
    return 0;
}