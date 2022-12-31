#include "packet_pairs.hpp"

List_t Pair1;
List_t Pair2;

void printPair() {
    printListInfo(&Pair1, 0);
    std::cout << " & ";
    printListInfo(&Pair2, 0);
}

void extractListInfo(std::istringstream *packet, List_t *l, int depth) {   
    char c, prev_c = '\0';
    Num_t num = 0;
    List_t *new_l = NULL;
    Contents_t cont;

    bool list_done = false;
    while (!list_done) {
        *packet >> c;
        switch (c) {
            case '[':
                new_l = newList();
                extractListInfo(packet, new_l, depth+1);
                cont = {list_t, new_l};
                l->push_back(cont);
                new_l = NULL;
                break;
            case ']':
                if ((prev_c != '\0') && (prev_c != ']')) {
                    cont = {num_t, newNum(num)};
                    l->push_back(cont);
                }
                list_done = true;
                break;
            case ',':
                if (prev_c != ']') {
                    cont = {num_t, newNum(num)};
                    l->push_back(cont);
                    num = 0;
                }
                break;
            default:
                num = (num * 10) + (c - '0');
                break;
        }
        prev_c = (c == '[' ? ']' : c);
    }
}

void extractPairInfo(std::string str, List_t *p) {
    std::istringstream packet(str);
    char c;
    packet >> c;
    extractListInfo(&packet, p, 0);
}

// returns how many pairs were in order
int evaluatePairs(std::ifstream *file, std::vector<int> *array) {
    std::string pair_str;
    int num_right = 0, right_index = 0;
    while (*file >> pair_str) {
        extractPairInfo(pair_str, &Pair1);
        *file >> pair_str;
        extractPairInfo(pair_str, &Pair2);

        right_index++;
        switch(listComparer(&Pair1, &Pair2)) {
            case -1:
                printPair(); std::cout << " are IN ORDER" << std::endl;
                num_right++;
                array->push_back(right_index);
                break;
            case  0:
                printPair(); std::cout << " are the SAME" << std::endl;
                break;
            case  1:
                printPair(); std::cout << " are OUT OF ORDER" << std::endl;
                break;
        }
        std::cout << " --- --- --- --- --- --- --- --- --- --- " << std::endl;
        
        deleteList(&Pair1);
        deleteList(&Pair2);
    }
    return num_right;
}

void typeChangeCleanup(Contents_t *content) {
    deleteList((List_t*)content->data);
    delete content;
}

// -1 if first is smaller, 0 if equal, 1 if second is bigger
int contentComparer(Contents_t *c1, Contents_t *c2) {
    int n1, n2;
    switch (c1->data_id) {
        case num_t:
            n1 = *((Num_t*)c1->data);
            n2 = *((Num_t*)c2->data);

            if (_DEBUG_PAIRS) { std::cout << "Comparing " << n1 << " & " << n2 << std::endl; }

            if (n1 < n2) { return -1; }
            else if (n1 == n2) { return 0; }
            else { return 1; }
            break;
        case list_t:
            if (_DEBUG_PAIRS) {
                std::cout << "Comparing ";
                printListInfo((List_t*)c1->data, 0);
                std::cout << " & ";
                printListInfo((List_t*)c2->data, 0);
                std::cout << std::endl;
            }

            return listComparer((List_t*)c1->data, (List_t*)c2->data);
            break;
    }
    return 0;   // to satisfy the compiler...
}

// -1 if first is smaller, 0 if equal, 1 if second is bigger
int listComparer(List_t* l1, List_t* l2) {
    size_t indx = 0;
    Contents_t *c1 = NULL, *c2 = NULL, *temp;
    bool type_changed = false, order_switched = false;
    int res = 0;
    while (indx < l1->size() && indx < l2->size()) {
        c1 = &(l1->at(indx));
        c2 = &(l2->at(indx));
        if (c1->data_id != c2->data_id) {
            if (c2->data_id == num_t) {
                temp = c2;
                c2 = c1;
                c1 = temp;
                order_switched = true;
            }
            c1 = numToList(c1);
            type_changed = true;
        }
        res = (order_switched ? contentComparer(c2, c1) : contentComparer(c1, c2));
        if (type_changed) {
            typeChangeCleanup(c1);
            type_changed = order_switched = false;
        }
        if (res != 0) { return res; }   // mismatch found
        indx++;
    }
    if (indx == l1->size()) {   // check which list ended first
        if (indx == l2->size()) {
            return 0;
        }
        else {
            return -1;
        }
    }
    else {
        return 1;
    }
}

//------------------------------------------------------------------------------

int arraySum(std::vector<int> *vec) {
    int sum = 0;
    for (int i : *vec) { sum += i; }
    return sum;
}

int main(int argc, char **argv) {
    if (argc <= 1 || argc > 2) {
        std::cout << "enter the filename while calling the program" << std::endl;
        return 1;
    }
    std::ifstream packets_file(argv[1], std::ios::in);
    if (!packets_file.is_open()) {
        std::cout << "cannot open " << argv[1] << std::endl;
        return 1;
    }

    std::vector<int> index_of_correct;
    index_of_correct.clear();

    int correct = evaluatePairs(&packets_file, &index_of_correct);
    std::cout << std::endl << "The number of packets ordered correctly : "
              << correct << std::endl;
    std::cout << "Sum of the indices of the correct pair : " 
              << arraySum(&index_of_correct) << std::endl;

    packets_file.close();
    return 0;
}

//------------------------------------------------------------------------------

void printListInfo(List_t *l, int depth) {
    if (l == NULL) { return; }
    std::cout << "[";
    for (Contents_t c : *l) {
        switch (c.data_id) {
            case num_t:
                std::cout << *((Num_t*)c.data) << ',';
                break;
            case list_t:
                printListInfo((List_t*)c.data, depth+1);
                break;
            default:
                std::cerr << "~Weird~";
        }
    }
    if (!l->empty()) { std::cout << "\b"; }
    std::cout << "]";
    if (depth) { std::cout << ",";}
}