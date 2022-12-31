#include <iostream>
#include <fstream>
#include <sstream>
#define HEADER_SIZE    4
#define MESSAGE_SIZE  14

#define UNIQUE_LEN MESSAGE_SIZE
char queue[UNIQUE_LEN];
int queue_size = 0;

void emptyQueue() {
    queue_size = UNIQUE_LEN - 1;
    while (queue_size >= 0) {
        queue[queue_size--] = '\0';
    }
}

void clearQueue(int index) {
    for (int i = index + 1; i <= UNIQUE_LEN; i++) {
        queue[i - index - 1] = queue[i];
    }
    queue_size -= index + 1;
}

int insertToQueue(char c) {
    for (int i = 0; i < queue_size; i++) {
        if (queue[i] == c) {
            clearQueue(i);
            break;
        }
    }
    queue[queue_size++] = c;
    return ((queue_size == (UNIQUE_LEN)) ? 0 : 1);
}

int main(int argc, char **argv) {
    if (argc <= 1 || argc > 2) {
        std::cout << "enter the filename while calling the program" << std::endl;
        return 1;
    }
    std::ifstream signal_data(argv[1], std::ios::in);
    if (!signal_data.is_open()) {
        std::cout << "cannot open " << argv[1] << std::endl;
        return 1;
    }
    std::string comm_message;
    while (signal_data >> comm_message) {
        std::cout << "Evaluating the packets in: " << comm_message.substr(0, 3) << "..." << comm_message.substr(comm_message.size()-3, comm_message.size()-1) << std::endl;
        
        std::istringstream comm_stream(comm_message);
        char ch = '\0';
        int i = 0;
        while (comm_stream >> ch) {
            if (!insertToQueue(ch)) {
                std::cout << "\tPacket at " << i+1 << std::endl;
                emptyQueue();
                break;
            }
            i++;
        }

        emptyQueue();
        std::cout << std::endl;
    }

    signal_data.close();
    return 0;
}
