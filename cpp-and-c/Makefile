C_FILES = main.c
CPP_LIB_FILES = lib.cpp
LIB = test

all: static dynamic

lib${LIB}.so:
	g++ -c ${CPP_LIB_FILES} -o $@ -fPIC -shared

lib${LIB}.a:
	g++ -c ${CPP_LIB_FILES} -o $@ -static

static: lib${LIB}.a
	gcc -o $@ ${C_FILES} lib${LIB}.a -lstdc++

dynamic: lib${LIB}.so
	gcc -o $@ ${C_FILES} -l${LIB} -lstdc++ -L. -Wl,-rpath=.

clean:
	rm static dynamic lib${LIB}.so lib${LIB}.a
