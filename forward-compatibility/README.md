# Future-compatibility in so-library
This example shows how a library can be made binary future compatible using
a single known function to request other functions. If a function we want to
use was added in a newer version of the library we will get a NULL-pointer back
and thus knows that the function is not availible.

The example library has two guarantied functions, `supported_functions` and
`get_function`. The first of them lists all availible functions and the second
returns a pointer given the name of the function. Except these the v1 version
has a function to generate prime numbers from 0 to X while the v2 lib has been
extended with a function to generate prime numbers from X to Y.

The binary compatibility of these construction allows us to:
- compile a program using libv1. This can now be run with either libv1 or libv2
- compile a program with libv2. This can now be run with libv2 or libv1 (with limited ability)

## Compile libs as shared objects...
```
gcc -g -O0 --shared -fPIC libv1.c -o libv1.so
gcc -g -O0 --shared -fPIC libv2.c -o libv2.so
```

## Compile and run the test program using lib version 1
```
gcc -g -O0  main.c -L. -lv1 -Wl,-rpath . && ./a.out
```

## Compile and run the test program using the extended lib version 2
```
gcc -g -O0  main.c -L. -lv2 -Wl,-rpath . && ./a.out
```

## Test binary compatibility
Compile you test program to use libv1, then rename libv2 to libv1:
```
gcc -g -O0  main.c -L. -lv1 -Wl,-rpath . && ./a.out
mv libv1.so lib_old.so && mv libv2.so libv1.so
./a.out
```
This shows binary backward-compatibility of the lib.

Compile the test program to use libv2, then rename the libv1 to libv2:
(Assuming you followed the renaming in last step)
```
mv libv1.so libv2.so
gcc -g -O0  main.c -L. -lv2 -Wl,-rpath . && ./a.out
mv libv2.so lib_new.so && mv lib_old.so libv2.so
./a.out
```
This shows forward compatibility! The testprogram fallbacks to the "primitive"
version of the prime-function.
