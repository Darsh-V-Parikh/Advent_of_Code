#ifndef _GROUND_MAP_H_
#define _GROUND_MAP_H_

#include<stdio.h>
#include<stdlib.h>
#include"vents.h"

void makeMap(int*** m, int xd, int yd) {
    printf("Dimensions for the map: %d x %d\n", xd, yd);
    /*make 2d array*/
    (*m) = malloc(xd * sizeof(int*));
    for(int i = 0; i < xd; i++) {
        (*m)[i] = malloc(sizeof(int) * yd);
        for (int j = 0; j < yd; j++) {
            (*m)[i][j] = 0;
        }
    }
}

void markMapPoint(int** m, VPoint* p, int* highest, int* high_count) {
    int* coord = &m[p->xp][p->yp];
    (*coord)++;
    if ((*coord) > (*highest)) {
        (*highest) = (*coord);
        (*high_count) = 1;
    }
    else if ((*coord) == (*highest)) { (*high_count)++; }
}

void deleteMap(int** m, int xd) {
    for (int i = 0; i < xd; i++) {
        free(m[i]);
    }
    free(m);
}

void printMap(int** m, int xd, int yd) {
    printf("Map of Hydrothermal vents:");
    for (int i = 0; i < xd; i++) {
        printf("\n\t");
        for (int j = 0; j < yd; j++) {
            printf("%d", m[i][j]);
        }
    }
    printf("\n");
}

#endif