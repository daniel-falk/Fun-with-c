#include "lib.h"

#include <stdio.h>
#include <stdlib.h>

prime_numbers_t *prime_numbers;
prime_numbers_range_t *prime_numbers_range;

void print_primes();

int main() {
    
    // Print list of availible functions
    int len, i;
    char **fncns = supported_functions(&len);
    for (i = 0; i < len; i++)
        printf("FUNCTION: %s\n", fncns[i]);
    printf("\n");


    // Set the function pointers
    prime_numbers       = get_function(FNCN_PRIME_NUMBERS);
    prime_numbers_range = get_function(FNCN_PRIME_NUMBERS_RANGE);

    if (!prime_numbers) {
        printf("Lib has no prime number function! Can't do anything...");
        return 1;
    }


    // Do some work and print all prime numbers between 100'000 and 100'100...
    print_primes((unsigned long)1e5, (unsigned long)1e5 + 100);


    // Clean up...
    for (i = 0; i < len; i++)
        free(fncns[i]);
    free(fncns);

    return 1;
}


// Print prime numbers between a and b
void print_primes(unsigned long a, unsigned long b) {
    int nums, i;
    unsigned long *numbers;

    if (prime_numbers_range)
        numbers = (*prime_numbers_range)(a, b, &nums); // Effective v2 solution
    else
        numbers = (*prime_numbers)(b, &nums); // Extremely slow, find all from 0...b

    for (i = 0; i < nums; i++)
        if (numbers[i] > a)
            printf("The %d:th prime number is: %lu\n", i, numbers[i]);
}
