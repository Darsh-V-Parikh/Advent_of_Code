#ifndef _PACKET_PAIR_HPP_
#define _PACKET_PAIR_HPP_

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#define _DEBUG_PAIRS 0

enum types_t { num_t = 0, list_t };

struct _list {
    types_t data_id;
    void *data;
};
typedef _list Contents_t;

typedef int Num_t;
typedef std::vector<Contents_t> List_t;

List_t* newList() {
    List_t* new_list = new List_t;
    new_list->clear();
    return new_list;
}

void deleteList(List_t *l) {
    if (l == NULL) { return; }
    for (Contents_t c : *l) {
        switch (c.data_id) {
            case num_t:
                delete (Num_t*)c.data;
                break;
            case list_t:
                deleteList((List_t*)c.data);
                delete (List_t*)c.data;
                break;
        }
    }
    l->clear();
}

void printListInfo(List_t *l, int depth);

Num_t* newNum(Num_t n) {
    Num_t *new_n = new Num_t;
    *new_n = n;
    return new_n;
}

Contents_t* numToList(Contents_t *old_content) {
    if (old_content == NULL) { return NULL; }
    Contents_t *new_list = new Contents_t;
    new_list->data_id = list_t;
    new_list->data = newList();
    ((List_t*)new_list->data)->push_back(*old_content);

    if (_DEBUG_PAIRS) {
        std::cout << "Changing " << *((Num_t*)old_content->data) << " into ";
        printListInfo((List_t*)new_list->data, 0);
        std::cout << std::endl;
    }

    return new_list;
}

bool numComparer(Num_t* n1, Num_t* n2) { return (*n1 > *n2 ? false : true); }

int listComparer(List_t* l1, List_t* l2);


#endif