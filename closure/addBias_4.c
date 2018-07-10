#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct addBias;
typedef int (*addBiasFunc_t)(struct addBias, int);

struct addBias {
    int bias;
    addBiasFunc_t func;
};

int addExactBiasFunc(struct addBias addBias, int value) {
    return value + addBias.bias;
}

int addNoisyBiasFunc(struct addBias addBias, int value) {
    return value + (((long)addBias.bias*rand())/RAND_MAX);
}

int addBias(struct addBias addBias, int value) {
    return addBias.func(addBias, value);
}

struct addBias addExactBiasCreate(int bias) {
    struct addBias b;
    b.bias = bias;
    b.func = addExactBiasFunc;

    return b;
}

struct addBias addNoisyBiasCreate(int bias) {
    struct addBias b;
    b.bias = bias;
    b.func = addNoisyBiasFunc;

    return b;
}

int main() {
    struct addBias addBias1 = addExactBiasCreate(1);
    struct addBias addBias20 = addNoisyBiasCreate(20);

    srand(time(NULL));

    printf("4 + 1 = %d\n", addBias(addBias1, 4));
    printf("10 + 1 = %d\n", addBias(addBias1, 10));
    printf("30 + ~20 = %d\n", addBias(addBias20, 30));

    return 0;
}

/*
 * Comnpile: gcc -Wall -Werror -pedantic addBias_4.c
 * Run: ./a.out
 *
 * Terminal output:
 * 4 + 1 = 5
 * 10 + 1 = 11
 * 30 + ~20 = 42
 */
