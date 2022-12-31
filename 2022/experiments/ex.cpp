#include <iostream>
#include <fstream>
#include <queue>

class Monkey {
        public:
    std::queue<int> curr_round_items;
    int operation;
    int worry_operand;
    int worry_threshold;
    int monkey_if_worried;
    int monkey_if_not_worried;
};

Monkey monk;
void assignItemsToMonkey(std::string items_str, Monkey *monkey);

void setupMonkeys(std::ifstream *monkey_file) {
    std::string str;
    char rand_chars;
    *monkey_file >> str >> str;

    // starting items
    *monkey_file >> str >> str;
    std::getline(*monkey_file, str);
    assignItemsToMonkey(str, &monk);
    // operation
    *monkey_file >> str >> str >> str >> str >> rand_chars >> str;
    if (str == "old") {
        monk.worry_operand = 0;
    }
    else {
        monk.worry_operand = atoi(str.c_str());
    }
    switch (rand_chars) {
        case '+':
            monk.operation = 1;
            break;
        case '*':
            monk.operation = 2;
            break;
    }
    // test and results
    *monkey_file >> str >> str >> str >> monk.worry_threshold;
    *monkey_file >> str >> str >> str >> str >> str 
                    >> monk.monkey_if_worried;
    *monkey_file >> str >> str >> str >> str >> str 
                    >> monk.monkey_if_not_worried;
}

void printMonkeyInfo(Monkey *monkey) {
    std::cout << "Operation : " << (monkey->operation == 1 ? "add" : "mul") << std::endl;
    std::cout << "Operand : " << monkey->worry_operand << std::endl;
    std::cout << "Test : " << monkey->worry_threshold << std::endl;
    std::cout << "If true : " << monkey->monkey_if_worried << std::endl;
    std::cout << "If false : " << monkey->monkey_if_not_worried << std::endl;
    std::cout << "Items : " << monkey->curr_round_items.size() << std::endl;
}

int main() {
    const char *file_name = "ex.txt";
    std::ifstream file(file_name);

    setupMonkeys(&file);
    printMonkeyInfo(&monk);

    file.close();
    return 0;
}



void assignItemsToMonkey(std::string items_str, Monkey *monkey) {
    std::cout << "checking items from: " << items_str << std::endl;
    int item = 0;
    for (char c : items_str) {
        switch (c) {
            case ' ':
                break;
            case ',':
                monkey->curr_round_items.push(item);
                item = 0;
                break;
            default:
                item = (item * 10) + (c - '0');
                break;
        }
    }
    monkey->curr_round_items.push(item);
}