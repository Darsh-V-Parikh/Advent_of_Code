#include "monkeys.h"

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+- OPERATIONS -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
int add(int a, int b) { return a+b; }
int mul(int a, int b) { return a*b; }
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// ! Program assumes the monkey's operation never has 0 as operand

typedef unsigned long Worry_t;
#define PUZZLE_PART 2   // 1 for Part 1, 2 for Part 2
std::vector<Monkey<Worry_t>> Monkeys;

void assignItemsToMonkey(std::string items_str, Monkey<Worry_t> *monkey) {
    while (!monkey->items.empty()) { monkey->items.pop(); }
    Worry_t item = 0;
    for (char c : items_str) {
        switch (c) {
            case ' ':
                break;
            case ',':
                monkey->items.push(item);
                item = 0;
                break;
            default:
                item = (item * 10) + (c - '0');
                break;
        }
    }
    monkey->items.push(item);
}

void setupMonkeys(std::ifstream *monkey_file) {
    std::string str;
    char rand_chars;
    while (*monkey_file >> str >> str) {
        Monkey<Worry_t> new_monkey;
        // starting items
        *monkey_file >> str >> str;
        std::getline(*monkey_file, str);
        assignItemsToMonkey(str, &new_monkey);
        // operation
        *monkey_file >> str >> str >> str >> str >> rand_chars >> str;
        if (str == "old") {
            new_monkey.worry_operand = 0;
        }
        else {
            new_monkey.worry_operand = atoi(str.c_str());
        }
        switch (rand_chars) {
            case '+':
                new_monkey.operation = add;
                break;
            case '*':
                new_monkey.operation = mul;
                break;
        }
        // test and results
        *monkey_file >> str >> str >> str >> new_monkey.worry_threshold;
        *monkey_file >> str >> str >> str >> str >> str 
                     >> new_monkey.monkey_if_worried;
        *monkey_file >> str >> str >> str >> str >> str 
                     >> new_monkey.monkey_if_not_worried;
        // done reading monkey info
        new_monkey.num_items_inspected = 0;
        Monkeys.push_back(new_monkey);
    }
}

//------------------------------------------------------------------------------

void printMonkeyInfo(Monkey<Worry_t> *monkey) {
    std::cout << "Operand : " << monkey->worry_operand << std::endl;
    std::cout << "Test : " << monkey->worry_threshold << std::endl;
    std::cout << "If true : " << monkey->monkey_if_worried << std::endl;
    std::cout << "If false : " << monkey->monkey_if_not_worried << std::endl;
    std::cout << "Items : " << monkey->items.size() << std::endl;
}

int main(int argc, char **argv) {
    if (argc <= 1 || argc > 2) {
        std::cout << "enter the filename while calling the program" << std::endl;
        return 1;
    }
    std::ifstream monkey_info(argv[1], std::ios::in);
    if (!monkey_info.is_open()) {
        std::cout << "cannot open " << argv[1] << std::endl;
        return 1;
    }

    int max_rounds;
    if (PUZZLE_PART == 1) {
        max_rounds = 20;
    }
    else if (PUZZLE_PART == 2) {
        max_rounds = 10000;
    }
    else {
        std::cerr << "Invalid Puzzle Part value : " << PUZZLE_PART 
                  << ". Must be either 1 or 2" << std::endl;
        std::cerr << "Change it by going to 'monkeys.cpp' and editing line 9" << std::endl;
        return 1;
    }

    setupMonkeys(&monkey_info);
    monkey_info.close();  
    // for (Monkey m : Monkeys) {
    //     printMonkeyInfo(&m);
    //     std::cout << std::endl;
    // }

    int round = 0;
    while (++round <= max_rounds) {
        for (int m = 0; m < Monkeys.size(); m++) {
            // std::cout << std::endl << " --- Monkey " << m << " ---" << std::endl;
            Monkeys[m].inspectItems();
        }        
        if ((PUZZLE_PART==1) || ((round==1) || (round==20) || (round%(max_rounds/10)==0))) {
            std::cout << std::endl << "   == After Round " << round << " ==" << std::endl;
            for (int m = 0; m < Monkeys.size(); m++) {
                fprintf(stdout, "Monkey %d inspected %6d items\n", m, Monkeys[m].num_items_inspected);
            }
        }
    }
    
    Worry_t max = 1, next_max = 0;
    for (Monkey<Worry_t> m : Monkeys) {
        if (m.num_items_inspected > max) {
            next_max = max;
            max = m.num_items_inspected;
        }
        else if (m.num_items_inspected > next_max) {
            next_max = m.num_items_inspected;
        }
        else {
            continue;
        }
    }
    std::cout << std::endl << "Monkey business: " 
              << max << " x " << next_max << " = " << (max*next_max) << std::endl;

    return 0;
}

//------------------------------------------------------------------------------

template <typename T>
void Monkey<T>::inspectItems() {
    while (!this->items.empty()) {
        T old_worry = this->items.front(), new_worry;
        if (this->worry_operand == 0) {
            new_worry = this->operation(old_worry, old_worry);
        }
        else {
            new_worry = this->operation(old_worry, this->worry_operand);
        }
        if (PUZZLE_PART == 1) {
            new_worry /= 3;
        }
        else if (PUZZLE_PART == 2) {
            //TODO  "Find a way to keep the worry managable" {Modulo is good for reductions..but what number?}
            //* Not INT_MAX or Monkeys.size[-1] ...
        }
        // std::cout << "Took " << old_worry << " and made it " << new_worry << std::endl;;
        if (new_worry % this->worry_threshold == 0) {
            Monkeys[this->monkey_if_worried].items.push(new_worry);
            // std::cout << "\tGave it to " << this->monkey_if_worried;
        }
        else {
            Monkeys[this->monkey_if_not_worried].items.push(new_worry);
            // std::cout << "\tGave it to " << this->monkey_if_not_worried;
        }
        this->items.pop();
        this->num_items_inspected++;
        // std::cout << std::endl;
    }
}

template <typename T>
void Monkey<T>::report(int monkey_id) {
    std::queue<T> temp = this->items;
    std::cout << "Monkey " << monkey_id << ':';
    while (!temp.empty()) {
        std::cout << ' ' << temp.front();
        temp.pop();
    }
    std::cout << std::endl;
}