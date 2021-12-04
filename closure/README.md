# Closure in c functions?

If you are used to JavaScript you know about function closure. A function in JavaScript can access all variables above its own scope, even in a parent function after it has closed. Eg. consider the generator for a funtion used to add a bias to a number:
```js
function addBiasX(x) {
    let func = function(value) {
        return x + value;
    }
  
    return func;
}

let addBias1 = addBiasX(1);
let addBias20 = addBiasX(20);

console.log("4 + 1 = " + (addBias1(4)).toString());
console.log("10 + 1 = " + (addBias1(10)).toString());
console.log("30 + 20 = " + (addBias20(30)).toString());

// Console output:
// 4 + 1 = 5
// 10 + 1 = 11
// 30 + 20 = 50
```

How would we do the same in ANSI C?

A common answer we might find on internet is that the C language does not support this behaviour, but that you can however do something like the following:
```c
/* File: addBias_1.c */

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
```

To avoid the repetition of the struct name we can either use the preprocessor:
```c
/* File: addBias_2.c */

#include <stdio.h>

#define ADD_BIAS(STRUCT, VALUE)    (STRUCT).func((STRUCT), (VALUE))  

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

    printf("4 + 1 = %d\n", ADD_BIAS(addBias1, 4));
    printf("10 + 1 = %d\n", ADD_BIAS(addBias1, 10));
    printf("30 + 20 = %d\n", ADD_BIAS(addBias20, 30));

    return 0;
}
```

...or we can introduce a new function:
```c
/* File: addBias_3.c */

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

int addBias(struct addBias addBias, int value) {
    return addBias.func(addBias, value);
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

    printf("4 + 1 = %d\n", addBias(addBias1, 4));
    printf("10 + 1 = %d\n", addBias(addBias1, 10));
    printf("30 + 20 = %d\n", addBias(addBias20, 30));

    return 0;
}
```

So far it might seem like there is no need to have the function pointer inside the struct, and therefore we wouldn't need to write the name of the struct twice any way... The use case is ofcourse that now we can allow different kinds of bias with different functions in the background but still hide it from the user. E.g. if we want to be able to choose from an exact bias (like we have had so far) or from a noisy bias:
```c
/* File: addBias_4.c */

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
```

So far it seems like a quite nice way to deal with it, C winns again! The problem arives when we want to pass our function to another API which will use it. When designing API's with a user defined callback in C it is common courtesy to include a `void *` for user defined data, this is one of the reasons. Consider the following case where we tell an library function to print a bunch of values using our bias function:
 ```c
 /* File: addBias_5.c */

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
 ```

 In this case we really got saved by the API-author and the fact that he provided us with a chain of user data pointers through the whole callback invocation. What if he was lazy, or for some other reason, didn't provide us with the user data pointer? One way is to make a new preprocessor function for each instance of the addBias function we have and make all addBias instances global so that we can use them from the preprocessor function.
```c
/* File: addBias_6.c */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
 * LIBRARY CODE FOR VERY USEFULL STUFF
 */

typedef int (*preprocessor_t)(int input_value);

int magic_numbers[] = {10, 100, 10, 50, 22, 8};

void print_magic_numbers(preprocessor_t preprocessor_func) {
    int i;
    for (i = 0; i < sizeof(magic_numbers) / sizeof(magic_numbers[0]); i++) {
        printf("Number %d is %d\n", i, preprocessor_func(magic_numbers[i]));
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
} addBias1, addBias20;

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

int bias_preprocessor_1(int input_value) {
    return addBias1.func(addBias1, input_value);
}

int bias_preprocessor_20(int input_value) {
    return addBias20.func(addBias20, input_value);
}

int main() {
    addBias1 = addExactBiasCreate(1);
    addBias20 = addNoisyBiasCreate(20);

    srand(time(NULL));

    printf("Values with bias = 1:\n");
    print_magic_numbers(bias_preprocessor_1);

    printf("Values with bias = ~20:\n");
    print_magic_numbers(bias_preprocessor_20);

    return 0;
}

/*
 * Comnpile: gcc -Wall -Werror -pedantic addBias_6.c
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
 * Number 0 is 16
 * Number 1 is 117
 * Number 2 is 13
 * Number 3 is 56
 * Number 4 is 28
 * Number 5 is 17
 */
```

 But what if we dynamically at runtime want to decide the number of addBias function variations we want? Since C is a compiled language we can't just create new functions when we need them. This whole problem is one of the things I dislike about C. However what I do like about C is that we can really do whatever we set our minds to, infact I was lying before. We can actually create new functions at runtime, we just need to know enough about the hardware the program is running on so that we can
 allocate a bit of memory with executable permision and generate some machine code which we put here. Next we can simply pass the address of the start of this memory bit as the callback function. This is however not an easy project, especially not if we want to support several platforms whith different architecture. That's why there are libraries out there we can make use of, e.g. callback in libffcall (FFCALL).
