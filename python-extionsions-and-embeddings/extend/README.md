## A simple C extension for python using the C API

There are multiple ways to create a C extension for python, the most low level is to use the Python C API which CPython itself is build on.

Take a look in the `simple_mod.c` file which implements a simple such module.

First it is recommended to create a virtual environment for python:
```bash
python -mvenv venv
source venv/bin/activate
```

Then install the python package, this will build the C extion module:
```bash
pip install -e .
```

You can now test run it with the following command. This will import the module and call the `print_list` method that will first block 10 seconds and then print the list:
```
python simple_run.py
```

You can try out the threaded example and see that the second python thread (the ticker) continues to run while the C extension is blocking the main python thread:
```bash
python threaded_run.py
```

This is not as obvious as one might first believe since CPython is not thread safe and has a global execution lock called the GIL (Global Interpreter Lock). The C extension does however have control over this lock and can realease it before calling any intense functions or performing any blocking IO. Try to comment out the lines `Py_BEGIN_ALLOW_THREADS` and `Py_END_ALLOW_THREADS` in the `simple_mod.c` file, then rebuild the module and run the script again:
```bash
pip install -e .
python threaded_run.py
```

You can now see that the ticker threads stops while the C function blocks. The reason that we can release the GIL before calling the blocking `cprint_list(p);` function is that we do not interact with any python objects or buffers. We release the GIL after we have parsed the python objects in the `print_list` function and aquires it again before touching any python objects. We are also copying the string buffers since the buffer returned by the `PyUnicode_AsUTF8` is managed by the python memory
management and is thus not safe to use after the GIL has been released.
