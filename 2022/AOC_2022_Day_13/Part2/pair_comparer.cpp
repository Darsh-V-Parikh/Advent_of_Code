#include "packet_pairs.hpp"

List_t* Message;
std::vector<List_t*> AllMessages;

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

void extractMessageInfo(std::string str) {
    std::istringstream packet(str);
    Message = newList();
    char c;
    packet >> c;
    extractListInfo(&packet, Message, 0);
}

size_t addMessageToList() {
    if (_DEBUG_INSERT) {
        std::cout << "Adding: "; printListInfo(Message, 0);
    }

    if (AllMessages.size() == 0) {
        if (_DEBUG_INSERT) { std::cout << " As the first element" << std::endl; }
        AllMessages.push_back(Message);
        return 0;
    }

    int low = 0, max = AllMessages.size() - 1, crit_indx = 0, high = max;
    while ((low < high) && (high >= 0) && (low <= max)) {    // search until the critical index is found
        crit_indx = (low + high) / 2;
        if (_DEBUG_INSERT) { std::cout << " \'" << crit_indx; }
        switch(listComparer(Message, AllMessages.at(crit_indx))) {
            case -1:    // message was smaller
                high = crit_indx - 1;
                if (_DEBUG_INSERT) { std::cout << "S\'"; }
                break;
            case  0:    // equal
                // can't be reached in this example... not too sure how to deal with this though
                break;
            case  1:    // message was bigger
                low = crit_indx + 1;
                if (_DEBUG_INSERT) { std::cout << "B"; }
                break;
        }
    }
    if (high < 0) {
        if (_DEBUG_INSERT) { std::cout << " As the first element" << std::endl; }
        AllMessages.insert(AllMessages.begin(), Message);
        return 0;
    }
    else if (high > max) {
        if (_DEBUG_INSERT) { std::cout << " As the last element" << std::endl; }
        AllMessages.push_back(Message);
        return AllMessages.size() - 1;
    }
    
    crit_indx = high; // or low... both are same
    if (listComparer(Message, AllMessages.at(crit_indx)) == 1) { // message is after the crit index
        crit_indx += 1;
    }
    if (_DEBUG_INSERT) { std::cout << " At " << crit_indx << std::endl; }
    AllMessages.insert(AllMessages.begin()+crit_indx, Message);
    return crit_indx;
}

// returns how many pairs were in order
void evaluateMessages(std::ifstream *file, std::vector<size_t> *array=NULL) {
    std::string message_str;
    size_t index;
    while (*file >> message_str) {
        extractMessageInfo(message_str);

        index = addMessageToList();
        if (array != NULL) {
            array->push_back(index+1);
        }
    }
}

//------------------------------------------------------------------------------

int main(int argc, char **argv) {
    if (argc <= 1 || argc > 3) {
        std::cout << "enter the name of the file with packets " 
                  << "and name of file with dividers "
                  << "while calling the program" << std::endl;
        return 1;
    }
    std::ifstream packets_file(argv[1], std::ios::in);
    std::ifstream dividers_file(argv[2], std::ios::in);
    if (!packets_file.is_open() || !dividers_file.is_open()) {
        std::cout << "cannot open " << argv[1] << std::endl;
        return 1;
    }

    std::vector<size_t> index_of_dividers;
    // index_of_dividers.clear();

    evaluateMessages(&packets_file);
    evaluateMessages(&dividers_file, &index_of_dividers);

    std::cout << "Decoder key = ";
    size_t ans = 1;
    for (size_t i : index_of_dividers) {
        std::cout << i << " x ";
        ans *= i;
    }
    std::cout << "\b\b= " << ans << std::endl;

    for (List_t* l : AllMessages) {
        deleteList(l);
    }
    packets_file.close();
    dividers_file.close();
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

void typeChangeCleanup(Contents_t *content) {
    delete content;
}

// -1 if first is smaller, 0 if equal, 1 if second is bigger
int contentComparer(const Contents_t *c1, const Contents_t *c2) {
    int n1, n2;
    switch (c1->data_id) {
        case num_t:
            n1 = *((Num_t*)c1->data);
            n2 = *((Num_t*)c2->data);

            if (_DEBUG_PAIRS) { std::cout << "Comparing " << n1 << " & " << n2 << std::endl; }

            if (n1 < n2) { return -1; }
            else if (n1 == n2) { return 0; }
            else { return 1; }
        case list_t:
            if (_DEBUG_PAIRS) {
                std::cout << "Comparing ";
                printListInfo((List_t*)c1->data, 0);
                std::cout << " & ";
                printListInfo((List_t*)c2->data, 0);
                std::cout << std::endl;
            }
            return listComparer((List_t*)c1->data, (List_t*)c2->data);
    }
    return 0;   // to satisfy the compiler...
}

// -1 if first is smaller, 0 if equal, 1 if second is smaller
int listComparer(List_t* l1, List_t* l2) {
    if (_DEBUG_PAIRS) {
        std::cout << "Comparing ";
        printListInfo(l1, 0);
        std::cout << " & ";
        printListInfo(l2, 0);
        std::cout << std::endl;
    }

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