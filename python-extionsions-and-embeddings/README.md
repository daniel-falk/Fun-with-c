## Python and C!

The most popular implementation of Python, named "CPython", is fully implemented in the C language. This means that it is possible to build extension libraries to Python which are implemented in C and deployed as shared objects. It is also posible to embed a python interpreter in a C (or C++) application.

There are multiple ways to build C extensions for python, the lowest level is to use the Python C API which CPython itself is build on. There are other libraries and tools available to make the wrapping easier, e.g. CFFI and Cython.

See the `extend` folder for an example explaining a simple extension using the C API.
