#ifndef _CAVE_HPP_
#define _CAVE_HPP_

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

enum obstacle_t { _Rock = 0, _Sand };

struct _cave_cell {
    size_t row_num;
    obstacle_t obstacle;
};
typedef _cave_cell Cell;

typedef std::vector<Cell> SortedRow_t;

struct _cave_row {
    size_t col_num;
    SortedRow_t Row;
};
typedef _cave_row SortedCol_t;

class Cave_t {
        public:
    int num_rock;
    int num_sand;

    Cave_t() : indx_col_last(-1), indx_row_last(-1), num_rock(0), num_sand(0) { 
        this->Interior.clear();
    }
    
    void addObstacle(int row, int col, obstacle_t obstacle);
    bool isOpen(int row, int col);

    // indicates the next row_num at which there is an obstacle within the specified col
    size_t nextObsInCol(int col, int row) {
        if (col == this->Interior[this->indx_col_last].col_num) {
            // !
        }
        else {
            size_t new_col = findCol(col);
            if (new_col == -1) { return -1; }

            // !
        }
    }


        private:
    std::vector<SortedCol_t> Interior;  // index of all obstacles
    size_t indx_col_last;               // index of the last accessed stuff
    size_t indx_row_last;               // changing columns will also change this value

    size_t findCol(int col);
    size_t findRow(int row, int col);

    size_t findColInsertable(int col);
    size_t findRowInsertable(int row, int col);
};

// returns -1 if no column, otherwise the index
size_t Cave_t::findCol(int col) {

}

// retuns -1 if no row, -2 if no column, otherwise the index
size_t Cave_t::findRow(int row, int col) {

}

// returns the index after which the column could be inserted
// returns -1 if the column exists, in which case indx_last contains the true index
size_t Cave_t::findColInsertable(int col) {

}

// returns the index after which the row could be inserted in the column specific
// returns -1 if the row exists, in which case indx_last contains the true index
// returns -2 if the column doesn't exist
size_t Cave_t::findRowInsertable(int row, int col) {

}


#endif