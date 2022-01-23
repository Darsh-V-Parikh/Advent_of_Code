#include<stdio.h>
#include<stdlib.h>
#include"board_list.h"

int* instructions;
int instr_sz, sum, multiplied, findSum(Board*);
int found_winner, remaining = 0;
Board* winner;
FILE* fp;
void getInstructions(), printInstructions(), tickOff1(int), tickOff2(int);


int main() {
    if (!(fp = fopen("bingo.txt", "r"))) {
        printf("File does not exist\n");
        return -1;
    }

    getInstructions();
    printInstructions();

    printf("Finding boards...\n");
    while(makeBoard(fp)) {
        // printf("Fetched another board\n");
        remaining++;
    }
    printf("_Found %d boards_\n", remaining);
    // printAllBoards();
    fclose(fp);

    int exercise;
    void (*tickOff)(int);
    printf("which exercise? (1 or 2) : ");
    scanf("%d", &exercise);
    if (exercise == 1) {
        printf("Finding the board that finishes first\n");
        tickOff = &tickOff1;
    } else if (exercise == 2) {
        printf("Finding the board that finishes last\n");
        tickOff = &tickOff2;
    }
    else {
        printf("No such exercise\n");
        return -1;
    }
    
    printf("\nStarting game\n");
    for(int i = 0; i < instr_sz; i++) {
        int val = instructions[i];
        printf("Number Called: %d\n", val);
        (*tickOff)(val);
        if (winner) {
            sum = findSum(winner);
            multiplied = sum * val;
            break;
        }
    }
    printf("\nWinner ");
    printBoard(winner);
    printf("Sum: %d   Number Called: %d   Multiplied: %d\n", 
            sum, (multiplied/sum), multiplied);
    
    saveAllBoard(fp);
    deleteBoards();
    free(instructions);
    return 0;
}

// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

void getInstructions() {
    /*count possible instructions*/
    instr_sz = 1;
    char c;
    while ((c = fgetc(fp)) != '\n') {
        if (c == ',') {instr_sz++;}
    }
    rewind(fp);
    /*store*/
    instructions = malloc(sizeof(int) * instr_sz);
    int n = 0, num = 0;
    while((c = fgetc(fp)) != '\n') {
        if (c == ',') {
            instructions[n] = num;
            n++;
            num = 0;
            continue;
        }
        num = (num * 10) + (c - '0');
    }
    instructions[n] = num;
    fgetc(fp); /*get the extra \n after the instructions*/
}

void printInstructions() {
    printf("Numbers: ");
    for(int i = 0; i < instr_sz; i++) {
        printf("%d, ", instructions[i]);
    }
    printf("\n");
}

void tickOff1(int number) {
    winner = NULL;
    Board* curr = headNode();
    while(curr) {
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                if(curr->board[i][j] == number) {
                    curr->board[i][j] = number ? (-1 * curr->board[i][j]) : -100;

                    curr->board[i][5] -= 1;
                    curr->board[5][j] -= 1;
                    if(curr->board[i][5] == 0 || curr->board[5][j] == 0) {
                        winner = curr;
                        return;
                    }
                }
            }
        }
        curr = curr->nextboard;
    }
}

void tickOff2(int number) {
    winner = NULL;
    Board* curr = headNode();
    while(curr) {
        if (curr->complete) {
            curr = curr->nextboard;
            continue;
        }
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                if((curr->board[i][j] == number)) {
                    curr->board[i][j] = number ? (-1 * curr->board[i][j]) : -100;

                    curr->board[i][5] -= 1;
                    curr->board[5][j] -= 1;
                    if(curr->board[i][5] == 0 || curr->board[5][j] == 0) {
                        curr->complete = 1;
                        remaining--;
                        found_winner = 1;
                        break;
                    }
                }
            }
            if (found_winner) {
                found_winner = 0;
                break;
            }
        }
        if (remaining < 1) {
            winner = curr;
            break;
        }
        curr = curr->nextboard;
    }
}

int findSum(Board* brd) {
    int s = 0;
    for(int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (brd->board[i][j] > 0) { s += brd->board[i][j]; }
        }
    }
    return s;
}