#include<stdlib.h>
#include<string.h>
#include<stdio.h>

FILE* fp;
int depth, pos, new, aim;
char instruction[100];
void exercise1(), exercise2();

int main() {
    fp = fopen("sub_instructions.txt", "r");
    if (!fp) {
        printf("File does not exist\n");
        return 1;
    }

    int ex;
    printf("Which exercise? (1 or 2): ");
    scanf("%d", &ex);

    if (ex == 1) {
        exercise1();
    }
    else if (ex == 2) {
        exercise2();
    }

    return 0;
}

void exercise1() {
    while (fscanf(fp, "%s %d\n", instruction, &new) != EOF) {
        if (!strcmp(instruction, "forward")) {
            pos += new;
        } else if (!strcmp(instruction, "up")) {
            depth -= new;
        } else if (!strcmp(instruction, "down")) {
            depth += new;
        }
    }

    printf("Depth: %d   Height: %d\n", depth, pos);
    printf("Multiplied: %d\n", (pos*depth));
}

void exercise2() {
    while (fscanf(fp, "%s %d\n", instruction, &new) != EOF) {
        if (!strcmp(instruction, "forward")) {
            pos += new;
            depth += (new * aim);
        } else if (!strcmp(instruction, "up")) {
            aim -= new;
        } else if (!strcmp(instruction, "down")) {
            aim += new;
        }
    }

    printf("Depth: %d   Height: %d\n", depth, pos);
    printf("Multiplied: %d\n", (pos*depth));
}