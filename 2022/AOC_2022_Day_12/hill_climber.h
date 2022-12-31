#ifndef _HILL_CLIMBER_H_
#define _HILL_CLIMBRE_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>

#define LOWEST_HEIGHT  'a'
#define HIGHEST_HEIGHT 'z'
#define MAX_CLIMBABLE   1

struct _location {
    char height;
    bool is_marked;
};
typedef _location Location_t;

struct _topology {
    int start_x, start_y;
    int end_x, end_y;
    std::vector< std::vector<Location_t> > Map;
};
typedef _topology Map_t;

void markLocation(int,int);
void unmarkLocation(int,int);

#define changeStdoutColor(clr) (SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), clr))
// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ COLORS +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
#define RED    4
#define WHITE  7
#define GREEN 10
#define GOLD   6

#endif