/* Include the python header file which declares the python C API functions */
#include <Python.h>
#include <string.h>

/*
 * This is a function that takes a null-terminated list of strings and prints them to
 * stdout. Before printing the strings it does a 10 seconds sleep to simulate some
 * blocking I/O or compute to show the effect of releasing the GIL or not.
 */
static void cprint_list(const char** p) {
    sleep(10);
    while (*p != NULL) {
        printf("%s ", *p);
        p++;
    }
    printf("\n");
}

/*
 * This function follows the python method API specification and will be callable from
 * the python domain after regestering it in the module. The python function expects
 * a single positional input argument, a list of strings. The real work is done by the
 * cprint_list function, this wrapper does however do the conversion from python objects
 * to regular c types and deals with any error handling.
 */
static PyObject* print_list(PyObject* self, PyObject* args) {
    /* Verify that the argument is a list and get the length. */
    PyObject* py_list;  // To keep the actual list (first argument) since args is a tuple with all arguments
    if (!PyArg_ParseTuple(args, "O!", &PyList_Type, &py_list)) {
        /* If argument is not a list, the PyErr string has been set by the parse function. */
        return NULL;
    }

    /* Verify that each member of the list is of type 'str'. */
    for (unsigned int i = 0; i < PyList_Size(py_list); ++i) {
        if (!PyUnicode_Check(PyList_GetItem(py_list, i))) {  /* Borrowed ref, no need to decrease ref cnt */
            PyErr_SetString(PyExc_TypeError, "must be called with a list of strings");
            return NULL;
        }
    }

    /* Convert the python object to a C style list of strings
     * The asUTF8 function caches the UTF-8 representation of the string in the python object,
     * there is thus no need to deallocate the buffer.
     */
    const char **p = malloc((PyList_Size(py_list) + 1) * sizeof(char *));  /* +1 for sentinel / null-terminator */
    for (unsigned int i = 0; i < PyList_Size(py_list); ++i) {
        p[i] = strdup(PyUnicode_AsUTF8(PyList_GetItem(py_list, i))); /* Copy buffer so that we can release the GIL */
    }
    p[PyList_Size(py_list)] = NULL; /* Sentinel */

    /* Call the C implementation which does the actual work, since this is a slow function
     * that will block for a long time we first release the GIL, the python global interpreter lock,
     * which allows other python threads to continue. We can to this since we do not interact with
     * any python objects or buffers in the c function.
     */
    Py_BEGIN_ALLOW_THREADS
    cprint_list(p);
    Py_END_ALLOW_THREADS

    /* Clean up the C buffer, all python references was borrowed */
    for (unsigned int i = 0; i < PyList_Size(py_list); ++i)
        free((void *)p[i]);
    free(p);

    /* This increments the ref count on python None object and returns it */
    Py_RETURN_NONE;
}

/*
 * This defines a set of puthon methods, it defines it to have one callable method wich
 * is callable by positional arguments (not key-word arguments).
 */
static PyMethodDef SpamMethods[] = {
    {"print_list", print_list, METH_VARARGS,
     "A function that prints a list of strings."},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

/* 
 * This defines the python module, the module has a name, a help text and a set of callable methods.
 * The lifetime of this struct needs to extend throughout the life of the python module, thus we
 * declare this staticly.
 */
static struct PyModuleDef spammodule = {
    PyModuleDef_HEAD_INIT,
    "spam",   /* name of module */
    "This is a spam module", /* module documentation */
    -1, /* -1 specifies the use of single-phase initialization */
    SpamMethods
};

/*
 * This is the module initialization function, it is called the first time the module is
 * imported in a python interpreter (once for each process). It follows the single-phase
 * initialization pattern, this is a legacy compatible pattern but should be used with care
 * since it might be problematic if there are subinterpreters etc. See more in PEP 489.
 * https://www.python.org/dev/peps/pep-0489/
 */
PyMODINIT_FUNC
PyInit_spam(void) {
    PyObject* m = PyModule_Create(&spammodule);
    printf("SPAM module created!\n");
    if (m == NULL) {
        return NULL;
    }
    return m;
}
