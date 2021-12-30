#include<stdlib.h>
#include<stdio.h>

FILE* fp;
int prev, new, count;
int num, buff[3], sum();
void exercise1(), exercise2(), compare();

int main() {

    fp = fopen("depths.txt", "r");
    if (!(fp)) {
        printf("Invalid Filename: No such file in directory");
        return 1;
    }

    printf("Which exercise (1 or 2) ? ");
    int ex;
    scanf("%d", &ex);
    ex -= 1;

    if (!ex) {
        exercise1();
    } else if (ex < 2) {
        exercise2();
    }
    else {
        printf("No exercise number %d available.\n", (ex+1));
        return -1;
    }

    return 0;
}

void exercise1() {
    fscanf(fp, "%d", &prev);
    while (fscanf(fp, "%d", &new) != EOF) {
        // printf("Comparing %d(prev) with %d(new)\n", prev, new);
        if (new > prev) { count++; }
        prev = new;
    }
    printf("Number of Increases: %d", count);
}

void exercise2() {
    while (num < 3) {
        fscanf(fp, "%d", (buff + num));
        num++;
    }
    if (!num == 2) {
        printf("Not enough data\n");
        return;
    }

    prev = sum();
    num = 0;
    while (fscanf(fp, "%d", (buff + num)) != EOF) {
        compare();
        num++;
        if (num > 2) { num = 0; }
    }
    if (num == 2) { compare(); }

    printf("Number of Increases: %d", count);
}

void compare() {
    new = sum();
    if (new > prev) { count++; }
    prev = new;
}

int sum() {
    int sum = 0;
    for(int i = 0; i < 3; i++) {
        sum += *(buff + i);
    }
    return sum;
}
