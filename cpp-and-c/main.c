#include <stdio.h>

extern void print_hey();
extern int  c_add_ab(int, int);

int main() {
    print_hey();

    printf("1 + 2 = %d\n", c_add_ab(1, 2));
}
