#include<stdio.h>
#include<stdlib.h>
#include"vents.h"
#include"groundmap.h"

FILE* fp;
int** map;
int xdim, ydim, over2, findIntermediates(VPoint*, VPoint);
int highpoint, hp_counter;
void markVents(int), findMapInfo(int);

int invalid(Vent* v, int ex_num) {
    if (!ex_num) {
        if(straightLineVent(v)) { return 0; }
        else { return 1; }
    }
}

int main() {
    fp = fopen("test.txt", "r");
    if (!fp) {
        printf("No such file in directory");
        return -1;
    }
    getVents(fp, &xdim, &ydim);
    makeMap(&map, xdim, ydim);

    markVents(0); // 0 for ex1, 1 for ex2
    findMapInfo(0);
    printf("More than two vents intersect at %d points\n", over2);
    printMap(map, xdim, ydim);

    deleteVents();
    deleteMap(map, xdim);
    return 0;
}

void markVents(int ex_num) {
    printf("Marking all the vents onto the map\n");
    Vent* vline = getHeadAddress();
    while (vline) {
        if(invalid(vline, ex_num)) {
            vline = vline->nextvent;
            continue;
        }
        VPoint new_point = vline->p1;
        markMapPoint(map, &new_point, &highpoint, &hp_counter);
        while (findIntermediates(&new_point, vline->p2)) {
            markMapPoint(map, &new_point, &highpoint, &hp_counter);
            if (isequal(new_point, vline->p2)) {break;}
        }
        vline = vline->nextvent;
    }
}

/*Find the next intermediate between the points and return it */
int findIntermediates(VPoint* cord1, VPoint cord2) {
    VPoint new;
    if (cord1->xp == cord2.xp) { 
        new.xp = cord1->xp;
        if (cord2.yp > cord1->yp) { new.yp = cord1->yp + 1; }
        else if (cord1->yp > cord2.yp) { new.yp = cord1->yp - 1; }
        else { new.yp = cord1->yp; }
    } else if (cord1->yp == cord2.yp) {
        new.yp = cord1->yp;
        if (cord2.xp > cord1->xp) { new.xp = cord1->xp + 1; }
        else if (cord1->xp > cord2.xp) { new.xp = cord1->xp - 1; }
        else { new.xp = cord1->xp; }
    }
    cord1->xp = new.xp;
    cord1->yp = new.yp;
    return 1;
}

void findMapInfo(int ex_num) {
    if (!ex_num) { /*exercise 2*/

    }
    else { /*exercise 1*/
        for(int i = 0; i < xdim; i++) {
            for(int j = 0; j < ydim; j++) {
                if(map[i][j] >= 2) { over2++; }
            }
        } 
    }
}