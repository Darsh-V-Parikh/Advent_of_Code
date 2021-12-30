#ifndef _VENTS_H_
#define _VENTS_H_

#include<stdlib.h>
#include<stdio.h>

#define TERMINATOR(k) ((k == ',') || (k == '\n') || (k == '>'))
#define NUM(k) ((k >= '0' && k <= '9'))
#define VALID(k) (NUM(k) || TERMINATOR(k) || (k == ' ') || (k == '-'))

typedef struct vent_point {
    int xp, yp;
} VPoint;

typedef struct vent_line {
    VPoint p1, p2;
    struct vent_line* nextvent;
} Vent;

Vent* Head = NULL;
Vent* Last = NULL;

VPoint* makeVPoint(int x, int y) {
    VPoint* vp = (VPoint*)malloc(sizeof(VPoint));
    vp->xp = x;
    vp->yp = y;
    return vp;
}

int isequal(VPoint p1, VPoint p2) {
    if (p1.xp == p2.xp && p1.yp == p2.yp) {return 1;}
    else {return 0;}
}

Vent* getHeadAddress() {
    return Head;
}

int addVentLine(Vent* v, int x, int y, int X, int Y) {
    if (!v) { return 0; }
    v->p1.xp = x;
    v->p2.xp = X;
    v->p1.yp = y;
    v->p2.yp = Y;
    return 1;
}

int straightLineVent(Vent* v) {
    if(v->p1.xp == v->p2.xp || v->p1.yp == v->p2.yp) { return 1; }
    else { return 0; }
}

Vent* makeVent(int x, int y, int X, int Y) {
    Vent* nw = (Vent*)malloc(sizeof(Vent));
    nw->nextvent = NULL;
    addVentLine(nw, x, y, X, Y);
    if (Head) {
        Last->nextvent = nw;
        Last = nw;
    }
    else { Head = Last = nw; }
    return Last;
}

void deleteVents() {
    Vent* next;
    while(Head) {
        next = Head->nextvent;
        free(Head);
        Head = next;
    }
}

void findVentEdges(int* max_X, int* max_Y) {
    *max_X += 1;
    *max_Y += 1;
}

int getVents(FILE* f, int* max_X, int* max_Y) {
    int x_1, x_2, y_1, y_2, second, num;
    x_1 = x_2 = y_1 = y_2 = num = second =0;
    char c;
    while ((c = fgetc(f)) != EOF) {
        if (!VALID(c)) {
            printf("Unknown character %c in input\n");
            return -1;
        }
        switch (c) {
            case ',':
                if (second) { 
                    x_2 = num;
                    if (x_2 > *max_X) {*max_X = x_2;}
                }
                else { 
                    x_1 = num;
                    if (x_1 > *max_X) {*max_X = x_1;}
                }
                num = 0;
                break;
            case '-':
                y_1 = num;
                if (y_1 > *max_Y) {*max_Y = y_1;}
                num = 0;
                break;
            case '>':
                second = 1;
                break;
            case '\n':
                if (num > *max_Y) {*max_Y = num;} 
                makeVent(x_1, y_1, x_2, num);
                num = second = 0;
                break;
            case ' ':
                break;
            default:
                num = num*10 + (c-'0');
                break;
        }
    }
    makeVent(x_1, y_1, x_2, num);
    
    findVentEdges(max_X, max_Y);
    return 0;
}

void printVentLine(Vent* v) {
    if (!v) {
        printf("Print Error: Non-existent vent\n");
        return;
    }
    printf("Line: %d,%d -> %d,%d\n", v->p1.xp, v->p1.yp, v->p2.xp, v->p2.yp);
}

void printAllVents() {
    Vent* curr = Head;
    printf("All the vent lines:\n");
    while (curr) {
        printVentLine(curr);
        curr = curr->nextvent;
    }
    printf("\n");
}

#endif