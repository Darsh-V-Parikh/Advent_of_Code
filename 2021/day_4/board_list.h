#ifndef _BOARD_LIST_H_
#define _BOARD_LIST_H_

typedef struct bingo_board {
    int complete;
    int board[6][6];
    struct bingo_board* nextboard;
} Board;

Board* make_board();

Board* headNode();

void deleteBoards();

int initBoard(FILE* p, Board* brd);

Board* start_board();

Board* add_board();

Board* makeBoard(FILE* f);

void saveAllBoard(FILE* f);

void printBoard(Board* brd);

void printAllBoards();

#endif