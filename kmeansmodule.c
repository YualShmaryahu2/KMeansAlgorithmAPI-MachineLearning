#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "kmeans.h"

static PyObject* fit(PyObject *self, PyObject *args) {
    PyObject *dataPoints, *centroids, *current_vector;
    int dim, N, num_of_clusters, iter;
    double epsilon;
    if (!PyArg_ParseTuple(args, "OOid", &dataPoints, &centroids, &iter, &epsilon)) {
        Py_RETURN_NONE;
    }
    N = PyObject_Length(dataPoints);
    dim = PyObject_Length(PyList_GetItem(dataPoints, 0));
    num_of_clusters = PyObject_Length(centroids);
    int i, j;
    double** input_data = (double**)calloc(N,sizeof (double*));
    double** array_of_centroids = (double**)calloc(num_of_clusters,sizeof (double*));
    for( i = 0; i < N; i++){
        input_data[i] = (double *) calloc(dim, sizeof(double));
        current_vector = PyList_GetItem(dataPoints, i);
        for(j = 0; j < dim; j++){
            input_data[i][j] = PyFloat_AsDouble(PyList_GetItem(current_vector, j));
        }
    }
    for( i = 0; i < num_of_clusters; i++){
        array_of_centroids[i] = (double *) calloc(dim, sizeof(double));
        current_vector = PyList_GetItem(centroids, i);
        for(j = 0; j < dim; j++){
            array_of_centroids[i][j] = PyFloat_AsDouble(PyList_GetItem(current_vector, j));
        }
    }
    array_of_centroids=kmeans(num_of_clusters, iter, input_data, array_of_centroids,N , dim,epsilon);

    PyObject* result = PyList_New(num_of_clusters);
    for (i = 0; i < num_of_clusters; i++){
        PyObject* lst = PyList_New(dim);
        for(j = 0; j < dim; j++){
            PyList_SetItem(lst, j, Py_BuildValue("d", array_of_centroids[i][j]));
        }
        PyList_SetItem(result, i, lst);
    }

    for(j=0; j < N; j++)
    {
        free(input_data[j]);
    }
    free(input_data);

    for(j=0; j < num_of_clusters; j++)
    {
        //free(array_of_centroids[j]);
    }
    //free(array_of_centroids);
    return result;
}

static PyMethodDef kmeansMethods[]={
            {"fit",
                (PyCFunction) fit,
                   METH_VARARGS,
                     PyDoc_STR("The method expects the following arguments in the following order:\n\n\
                            \t^datapoints = an array of all datapoints\n\
                            \tin which each cell in the array is an array of doubles\n\
                            \n\
                            \t^centroids = an array of initial centroids\n\
                            \tin which each cell is an array of doubles\n\
                            \n\
                             t^iter = an integer which represents the number of K-means iterations\n\
                            \n\
                            \t^eps = a double which represents the epsilon value for convergence")
        },
        {NULL,NULL,0,NULL}
};

static struct PyModuleDef kmeansModule = {
        PyModuleDef_HEAD_INIT,
        "mykmeanssp",
        NULL,
        -1,
        kmeansMethods
};

PyMODINIT_FUNC
PyInit_mykmeanssp(void)
{
    PyObject *m;
    m=PyModule_Create(&kmeansModule);
    if(!m)
    {
        Py_RETURN_NONE;
    }
    return m;
}
