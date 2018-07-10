#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
 * LIBRARY CODE FOR VERY USEFULL STUFF
 */

typedef int (*preprocessor_t)(int input_value, void *user_data);

int magic_numbers[] = {10, 100, 10, 50, 22, 8};

void print_magic_numbers(preprocessor_t preprocessor_func, void *user_data) {
    int i;
    for (i = 0; i < sizeof(magic_numbers) / sizeof(magic_numbers[0]); i++) {
        printf("Number %d is %d\n", i, preprocessor_func(magic_numbers[i], user_data));
    }
}


/*
 * OUR CODE
 */

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

int bias_preprocessor(int input_value, void *user_data) {
    struct addBias *addBias = (struct addBias *)user_data;
    return addBias->func(*addBias, input_value);
}

int main() {
    struct addBias addBias1 = addExactBiasCreate(1);
    struct addBias addBias20 = addNoisyBiasCreate(20);

    srand(time(NULL));

    printf("Values with bias = 1:\n");
    print_magic_numbers(bias_preprocessor, (void *)&addBias1);

    printf("Values with bias = ~20:\n");
    print_magic_numbers(bias_preprocessor, (void *)&addBias20);

    return 0;
}

/*
 * Comnpile: gcc -Wall -Werror -pedantic addBias_5.c
 * Run: ./a.out
 *
 * Terminal output:
 * Values with bias = 1:
 * Number 0 is 11
 * Number 1 is 101
 * Number 2 is 11
 * Number 3 is 51
 * Number 4 is 23
 * Number 5 is 9
 * Values with bias = ~20:
 * Number 0 is 11
 * Number 1 is 115
 * Number 2 is 10
 * Number 3 is 59
 * Number 4 is 30
 * Number 5 is 8
 */
