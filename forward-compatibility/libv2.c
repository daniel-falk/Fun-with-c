#include "lib.h"

#include <stdlib.h>
#include <string.h>

// ===============================================================
// ========= Forward declaration of availible functions ==========
// ===============================================================


prime_numbers_t __prime_numbers;
prime_numbers_range_t __prime_numbers_range;


// ===============================================================
// ===== THE FOLLOWING 2 FUNCTIONS ARE GUARANTIED BY THE API =====
// ===============================================================


// Get the list of all functions supplied by this library
char **supported_functions(int *length) {
    *length = 2;

    char **fncns = malloc(sizeof(char *) * 2);
    fncns[0] = strdup(FNCN_PRIME_NUMBERS);
    fncns[1] = strdup(FNCN_PRIME_NUMBERS_RANGE);

    return fncns;
}


// Get the address of a function known by name
void *get_function(char *name) {
    if (strcmp(name, FNCN_PRIME_NUMBERS) == 0)
        return (void *)__prime_numbers;
    else if (strcmp(name, FNCN_PRIME_NUMBERS_RANGE) == 0)
        return (void *)__prime_numbers_range;
    else
        return NULL;
}


// ===============================================================
// === THE FOLLOWING FUNCTIONS ARE NOT GUARANTIED/KNOWN BY API ===
// ===============================================================


// Get all prime numbers from 0...max
unsigned long *__prime_numbers(unsigned long max, int *num_numbers) {
    __prime_numbers_range(1, max, num_numbers);
}


// Get all prime numbers between min and max
unsigned long *__prime_numbers_range(unsigned long min, unsigned long max, int *num_numbers) {

    unsigned long i;
    unsigned long *out = NULL;

    *num_numbers = 0;

    while (min < max)
    {
        for(i = 2; i <= min/2; ++i)
            if(min % i == 0)
                break;

        if (i > min/2) {
            out = realloc(out, sizeof(unsigned long) * ++(*num_numbers));
            out[*num_numbers - 1] = min;
        }

        ++min;
    }

    return out;
}
