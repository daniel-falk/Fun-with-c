
// Get the list of all functions supplied by this library
char **supported_functions(int *length);

// Get the address of a function known by name
void *get_function(char *name);

// ===============================================================
// ======== THE FOLLOWING FUNCTIONS MAY EXIST IN A LIBRARY =======
// ===============================================================

// Get all prime numbers from 0...max
#define FNCN_PRIME_NUMBERS "prime_numbers"
typedef unsigned long *prime_numbers_t(unsigned long max, int *num_numbers);

// Get all prime numbers between min and max
#define FNCN_PRIME_NUMBERS_RANGE "prime_numbers_range"
typedef unsigned long *prime_numbers_range_t(unsigned long min, unsigned long max, int *num_numbers);
