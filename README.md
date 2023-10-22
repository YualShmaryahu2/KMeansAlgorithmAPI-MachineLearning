# KMeansAlgorithmAPI-MachineLearning
The K-means++ algorithm is used to choose initial centroids for the K-means algorithm. In this repository this algorithm will be implemented in Python and integrated with the K-means algorithm of KMeansAlgorithm-MachineLearning that will be ported to a C extension using the C API.
------------------------------------------------------------------------------------------------------------------------------------------
in this repository there are the following files:

kmeans_pp.py: 
This is the interface of the program, with the following requirements: (Reading user CMD arguments)
• k: Number of required clusters.
• iter: (Optional) argument determines the number of K-means iterations, if not provided the default value is 300.
• eps: the epsilob value for convergence.
• file_name_1: The path to the file 1 that will contain N observations.
• file_name_2: The path to the file 2 that will contain N observations.


kmeansmodule.c:
In this file the C extension is defined.

setup.py:
This is the build used to create the *.so file that will allow kmeans_pp.py to import mykmeanssp.

kmeans.c:
The implementation of KMeans algorithn in C.

kmeans.h:
The Header file that inteface between kmeansmodule.c and kmeans.c.
