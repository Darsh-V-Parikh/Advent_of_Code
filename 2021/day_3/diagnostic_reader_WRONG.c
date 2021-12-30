#include<stdlib.h>
#include<stdio.h>

#define NUM(c) (c - '0')

FILE* fp;
int gamme, epsilon, zero, one, common, bin_len, line;
int O2, CO2;
char binary[100];
char* possible;
int* g; 
int* e;
int* o2;
int* co2;
void exercise1(), exercise2();

void findColumns(int* length) {
    fscanf(fp, "%s", binary);
    rewind(fp);
    for((*length) = 0; binary[*length] != '\0'; (*length)++);
}

int main() {
    fp = fopen("diag.txt", "r");
    if (!fp) {
        printf("File does not exist\n");
        return 1;
    }
    findColumns(&bin_len);
    g = malloc(sizeof(int)*bin_len);
    e = malloc(sizeof(int)*bin_len);
    o2 = malloc(sizeof(int)*bin_len);
    co2 = malloc(sizeof(int)*bin_len);

    exercise1();
    exercise2();
    
    free(g);
    free(e);
    free(o2);
    free(co2);
    return 0;
}

int mostCommon(int index) {
    line = 1;
    while (fscanf(fp, "%s", binary) != EOF) {
        // printf("%c\n", binary[i]);
        if(binary[index] - '0') {one++;}
        else {zero++;}
        line++; // for exercise2
    }
    rewind(fp);
    return ((one > zero) ? 1 : 0);
}

void exercise1() {
    printf("\t\t\tExercise 1\n");
    for(int i = 0; i < bin_len; i++) {
        common = mostCommon(i);
        g[i] = common; // for exercise2
        e[i] = (common ? 0 : 1); // for exercise2
        one = zero = 0;
        // printf("Common: %d\n", common);
        
        gamme = (gamme * 2) + common;
        epsilon = (epsilon * 2) + (common ? 0 : 1);
    }

    printf("Gamma Value: ");
    for (int alph = 0; alph < bin_len; alph++) {
        printf("%d", g[alph]);
    }
    printf(" (%d)   Epsilon: ", gamme);
    for (int alph = 0; alph < bin_len; alph++) {
        printf("%d", e[alph]);
    }
    printf(" (%d)\n", epsilon);
    printf("Multiplied: %d\n", (gamme*epsilon));
}

char* getFromArray(int row, int clmn) {
    return &(possible[bin_len*row + clmn]);
}

void copyBinaryFile(int current_idx) {
    for(int alph = 0; alph < bin_len; alph++) {
        *(getFromArray(current_idx, alph)) = binary[alph];
    }
}

void copyBinaryArray(int current_idx, int copier_idx) {
    for(int alph = 0; alph < bin_len; alph++) {
        *getFromArray(current_idx, alph) = *getFromArray(copier_idx, alph);
    }
}

void printCurrentWorking(int size) {
    printf("Pool: [");
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < bin_len; j++) {
            printf("%c", *(getFromArray(i, j)));
        }
        printf(", ");
    }
    printf("(size: %d)]\n", size);
}

void _ex2_alg(int o) {
    o ? printf("O2\n") : printf("CO2\n");
    int p_idx = 0, p_size, nw_idx;
    
    /* put the valid binaries into the array based on the first bit */
    for(int l = 0; l < line; l++) {
        fscanf(fp, "%s", binary);
        if ((binary[0]-'0') == (o ? g[0] : e[0])) {
            copyBinaryFile(p_idx);
            p_idx++;
        }
    }
    rewind(fp);
    p_size = p_idx;
    // printCurrentWorking(p_size);

    /* get the rest from the array itself, storing back into the array */
    for(int idx = 1; (idx < bin_len) && (p_size > 1); idx++) {
        // printf("checking position %d; Must be: %d \n", idx, (o ? g[idx] : e[idx]));
        for(p_idx = nw_idx = 0; p_idx < p_size; p_idx++) {
            char c = *getFromArray(p_idx, idx);
            if (c - '0' == (o ? g[idx] : e[idx])) {
                copyBinaryArray(nw_idx, p_idx);
                nw_idx++;
            }
        }
        p_size = nw_idx;
        // printCurrentWorking(p_size);
    }

    /* Save values */
    for(int alph = 0; alph < bin_len; alph++) {
        if (o) {
            o2[alph] = *(getFromArray(0, alph)) - '0';
        }
        else {
            co2[alph] = *(getFromArray(0, alph)) - '0';
        }
    }
}

void exercise2() {
    printf("\t\t\tExercise 2\n");
    possible = malloc(sizeof(char)*bin_len*line);
    _ex2_alg(1);
    _ex2_alg(0);

    /* print from arrays */
    int nw = 0;
    printf("O2 levels: ");
    for(int i = 0; i < bin_len; i++) {
        nw = o2[i];
        printf("%d", nw);
        O2 = (O2 * 2) + nw;
    }
    printf(" (%d)   CO2 levels: ", O2);
    for(int i = 0; i < bin_len; i++) {
        nw = co2[i];
        printf("%d", nw);
        CO2 = (CO2 * 2) + nw;
    }
    printf(" (%d)\n", CO2);
    printf("Multiplied: %d", (CO2*O2));
    
    free(possible);
}