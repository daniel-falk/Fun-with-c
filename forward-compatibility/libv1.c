#include "lib.h"

#include <stdlib.h>
#include <string.h>

// ===============================================================
// ========= Forward declaration of availible functions ==========
// ===============================================================


prime_numbers_t __prime_numbers;


// ===============================================================
// ===== THE FOLLOWING 2 FUNCTIONS ARE GUARANTIED BY THE API =====
// ===============================================================


// Get the list of all functions supplied by this library
char **supported_functions(int *length) {
    *length = 1;

    char **fncns = malloc(sizeof(char *));
    fncns[0] = strdup(FNCN_PRIME_NUMBERS);

    return fncns;
}


// Get the address of a function known by name
void *get_function(char *name) {
    if (strcmp(name, FNCN_PRIME_NUMBERS) == 0)
        return (void *)__prime_numbers;
    else
        return NULL;
}


// ===============================================================
// === THE FOLLOWING FUNCTIONS ARE NOT GUARANTIED/KNOWN BY API ===
// ===============================================================


// Get all prime numbers from 0...max
unsigned long *__prime_numbers(unsigned long max, int *num_numbers) {

    unsigned long i0 = 1, i;
    unsigned long *out = NULL;

    *num_numbers = 0;

    while (i0 < max)
    {
        for(i = 2; i <= i0/2; ++i)
            if(i0 % i == 0)
                break;

        if (i > i0/2) {
            out = realloc(out, sizeof(unsigned long) * ++(*num_numbers));
            out[*num_numbers - 1] = i0;
        }

        ++i0;
    }

    return out;
}
