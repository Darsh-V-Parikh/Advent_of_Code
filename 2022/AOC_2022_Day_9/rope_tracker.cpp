#include <iostream>
#include <fstream>
#include <vector>

#define VERBOSE 0   // setting this to 1 prints details useful for debugging

struct _2dPoint {
    int x;
    int y;

    bool operator==(struct _2dPoint other) {
        return (other.x == x && other.y == y);
    }
};
typedef _2dPoint Point_t;

struct _knot {
    int knot_id;
    Point_t loc;
    struct _knot *next;
    struct _knot *prev;
};
typedef _knot Knot_t;

#define ROPE_LEN 10
#define START_POINT {0,0}
Knot_t *Head;
std::vector<Point_t> VisitedPoints, UniqueVisitedPoints;

void makeRope(int num_knots, Point_t start_point);
void deleteRope();

char directionality(Knot_t *knot) {
    if (knot->prev->loc.y > knot->loc.y) {
        if (knot->prev->loc.x > knot->loc.x) {
            return 'r';
        }
        else {
            return 'u';
        }
    }
    else {
        if (knot->prev->loc.x > knot->loc.x) {
            return 'd';
        }
        else {
            return 'l';
        }
    }
}

void _moveKnot(Knot_t *knot, char direc, bool diag = false) {
    if (VERBOSE) {
        std::cout << " : Moving " << direc << (diag ? " diagonally" : "           ");
    }
    switch (direc) {
        case 'U':
            knot->loc.y++;
            if (diag) {
                knot->loc.x--;
            }
            break;
        case 'D':
            knot->loc.y--;
            if (diag) {
                knot->loc.x++;
            }
            break;
        case 'L':
            knot->loc.x--;
            if (diag) {
                knot->loc.y--;
            }
            break;
        case 'R':
            knot->loc.x++;
            if (diag) {
                knot->loc.y++;
            }
            break;
    }
    if (knot->next == NULL) { // knot is the Tail
        VisitedPoints.push_back(knot->loc);
    }
}

bool moveKnot(char direction, Knot_t *knot) {
    Knot_t *prev_k = knot->prev;
    int x_dist = 0, y_dist = 0;
    if (knot->loc.x == prev_k->loc.x) { // same column
        y_dist = knot->loc.y - prev_k->loc.y;
        if (VERBOSE) {
            fprintf(stdout, "Knot %2d : Same Column | y= %2d         ", knot->knot_id, y_dist);
        }
        if (abs(y_dist) <= 1) {
            if (VERBOSE) { std::cout << " : NO MOTION"; }
            return false;
        }
        else {
            _moveKnot(knot, (y_dist > 0 ? 'D' : 'U'));
            return true;
        }
    }
    else if (knot->loc.y == prev_k->loc.y) { // same Row
        x_dist = knot->loc.x - prev_k->loc.x;
        if (VERBOSE) {
            fprintf(stdout, "Knot %2d : Same Row    | x= %2d         ", knot->knot_id, x_dist);
        }
        if (abs(x_dist) <= 1) {
            if (VERBOSE) { std::cout << " : NO MOTION"; }
            return false;
        }
        else {
            _moveKnot(knot, (x_dist > 0 ? 'L' : 'R'));
            return true;
        }
    }
    else { // Diagnoal
        x_dist = knot->loc.x - prev_k->loc.x;
        y_dist = knot->loc.y - prev_k->loc.y;
        if (VERBOSE) {
            fprintf(stdout, "Knot %2d : Diagonal    | x= %2d , y= %2d ", knot->knot_id, x_dist, y_dist);
        }
        if ((abs(y_dist) <= 1) && (abs(x_dist) <= 1)) {
            if (VERBOSE) { std::cout << " : NO MOTION"; }
            return false;
        }
        switch (directionality(knot)) {
            case 'u':
                _moveKnot(knot, 'U', true);
                break;
            case 'd':
                _moveKnot(knot, 'D', true);
                break;
            case 'l':
                _moveKnot(knot, 'L', true);
                break;
            case 'r':
                _moveKnot(knot, 'R', true);
                break;
        }
        return true;
    }
}

void moveRope(char direction) {
    // move head
    switch (direction) {
        case 'U':
            Head->loc.y++;
            break;
        case 'D':
            Head->loc.y--;
            break;
        case 'L':
            Head->loc.x--;
            break;
        case 'R':
            Head->loc.x++;
            break;
    }

    Knot_t *curr = Head->next;
    bool move_next = true;
    
    while (curr && move_next) {
        move_next = moveKnot(direction, curr);
        curr = curr->next;
        if(VERBOSE) { std::cout << std::endl; }
    }
}

void printRopeStats();
void printStats();
void findUniquePoints();

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-

int main(int argc, char **argv) {
    if (argc <= 1 || argc > 2) {
        std::cout << "enter the filename while calling the program" << std::endl;
        return 1;
    }
    std::ifstream rope_motion(argv[1], std::ios::in);
    if (!rope_motion.is_open()) {
        std::cout << "cannot open " << argv[1] << std::endl;
        return 1;
    }

    makeRope(ROPE_LEN-1, START_POINT);
    VisitedPoints.push_back(START_POINT);
    char direction;
    int amount;
    while (rope_motion >> direction >> amount) {
        if (VERBOSE) {
            std::cout << "\t\t== " << direction << " " << amount << " ==" << std::endl;
        }
        while (amount--) {
            moveRope(direction);
            if (VERBOSE) { printRopeStats(); }
        }
        if (VERBOSE) { std::cout << std::endl; }
    }
    findUniquePoints();

    printStats();

    deleteRope();
    rope_motion.close();
    return 0;
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-

// excluding Head. Head is always there. num_knots = 1 ==> H -> T
void makeRope(int num_knots, Point_t start_point) {
    Head = new Knot_t;
    Head->knot_id = 0;
    Head->loc = start_point;
    Head->prev = NULL;
    Knot_t *curr = Head;
    for (int i = 0; i < num_knots; i++) {
        curr->next = new Knot_t;
        curr->next->prev = curr;
        curr = curr->next;
        curr->loc = start_point;
        curr->knot_id = curr->prev->knot_id + 1;
    }
    curr->next = NULL;

    std::cout << " Your rope: H";
    curr = Head->next;
    while (curr) {
        std::cout << " -> " << curr->knot_id;
        curr = curr->next;
    }
    std::cout << std::endl;
}

void deleteRope() {
    Knot_t *curr = Head, *tmp;
    while (curr) {
        tmp = curr->next;
        delete curr;
        curr = tmp;
    }
}

void findUniquePoints() {
    bool already_present;
    for (Point_t p : VisitedPoints) {
        already_present = false;
        for (Point_t up : UniqueVisitedPoints) {
            if (p == up) {
                already_present = true;
                break;
            }
        }
        if (!already_present) {
            UniqueVisitedPoints.push_back(p);
        }
    }
}

void printRopeStats() {
    Knot_t *curr = Head;
    while (curr->next) {
        fprintf(stdout, "{%2d,%2d} -> ", curr->loc.x, curr->loc.y);
        curr = curr->next;
    }
    fprintf(stdout, "{%2d,%2d}\n", curr->loc.x, curr->loc.y);
}

void printStats() {
    std::cout << " --+--+-- --+--+-- --+--+--" << std::endl;
    std::cout << "Details about the Tail's traversal:" << std::endl;
    std::cout << "\tTotal Points:  " << VisitedPoints.size() << std::endl;
    std::cout << "\tUnique Points: " << UniqueVisitedPoints.size() << std::endl;
}