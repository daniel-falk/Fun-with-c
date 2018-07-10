#include <stdio.h>

struct addBias;
typedef int (*addBiasFunc_t)(struct addBias, int);

struct addBias {
    int bias;
    addBiasFunc_t func;
};

int addBiasFunc(struct addBias addBias, int value) {
    return value + addBias.bias;
}

struct addBias addBiasCreate(int bias) {
    struct addBias b;
    b.bias = bias;
    b.func = addBiasFunc;

    return b;
}

int main() {
    struct addBias addBias1 = addBiasCreate(1);
    struct addBias addBias20 = addBiasCreate(20);

    printf("4 + 1 = %d\n", addBias1.func(addBias1, 4));
    printf("10 + 1 = %d\n", addBias1.func(addBias1, 10));
    printf("30 + 20 = %d\n", addBias20.func(addBias20, 30));

    return 0;
}

/*
 * Compile: gcc -Wall -Werror -pedantic addBias_1.c
 * Run: ./a.out
 *
 * Terminal output:
 * 4 + 1 = 5
 * 10 + 1 = 11
 * 30 + 20 = 50
 */
