#include <iostream>

// Directly declaring a c++ function with no mangling:
extern "C"
void print_hey() {
    std::cout << "hello from c++! =)" << std::endl;
}

// Wrapping a c++ function with a function declared as extern "C":
int add_ab(int a, int b) {
    return a + b;
}

extern "C" int c_add_ab(int, int);
int c_add_ab(int a, int b) {
    return add_ab(a, b);
}
