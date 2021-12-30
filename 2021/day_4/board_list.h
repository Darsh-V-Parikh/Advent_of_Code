#ifndef _BOARD_LIST_H_
#define _BOARD_LIST_H_

#include<stdio.h>
#include<stdlib.h>

typedef struct bingo_board {
    int complete;
    int board[6][6];
    struct bingo_board* nextboard;
} Board;

Board* Head = NULL;
Board* Last = NULL;
int no_more = 0;

Board* make_board() {
    Board* nw_b = (Board*)malloc(sizeof(Board));
    nw_b->nextboard = NULL;
    nw_b->complete = 0;
    return nw_b;
}

Board* deleteBoards() {
    Board* start = Head;
    Board* next;
    while(start) {
        next = start->nextboard;
        free(start);
        start = next;
    }
}

int initBoard(FILE* p, Board* brd) {
    for(int i = 0; i < 5; i++) {
        for(int j = 0; j < 5; j++) {
            fscanf(p, "%d", &(brd->board[i][j]));
            // printf("%d\n", brd->board[i][j]);
        }
        brd->board[i][5] = 5;
    }
    for(int i = 0; i < 6; i++) {
        brd->board[5][i] = 5;
    }
    /*return the next character.*/
    return fgetc(p); /*Remove it from the stack (each board has 2 '\n' after them)*/
}

Board* start_board() {
    Board* another_board = make_board();
    Head = Last = another_board;
    return another_board;
}

Board* add_board() {
    Board* another = make_board();
    Last->nextboard = another;
    return (Last = another);
}

Board* makeBoard(FILE* f) {
    if (!no_more) {
        Board* newboard = (Head ? add_board() : start_board());
        if (initBoard(f, newboard) == EOF) {
            no_more = 1;
        }
        return newboard;
    }
    else {
        printf("No more boards...\n");
        return NULL;
    }
}

void printBoard(Board* brd) {
    // if (Head == brd) {printf("First ");}
    // else if (Last == brd) {printf("Last ");}
    // else {printf("Next ");}
    printf("Board:\n");
    for(int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            printf("%.2d ", brd->board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void printAllBoards() {
    Board* curr = Head;
    int counter = 0;
    while (curr) {
        printBoard(curr);
        curr = curr->nextboard;
        // counter++;
    }
    // ("Total of %d boards\n", counter);
}

#endif