#ifndef _MONKEYS_H_
#define _MONKEYS_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>

template <typename T>
class Monkey {
        public:
    std::queue<T> items;
    int (*operation)(int, int);
    int worry_operand;
    int worry_threshold;
    int monkey_if_worried;
    int monkey_if_not_worried;
    T num_items_inspected;

    // returns the index of the monkey to which the new value is passed
    void inspectItems();
    void report(int);
};

#endif