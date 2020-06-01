#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <cstring>

#define N_CL 5          // number of clusters
#define M 2             // fuzzification parameter
#define DIM 2         // dimension of the point
#define EPS 0.001     // threshold to stop the algorithm
#define N_ITER 10000 // max number of iterations

// points per cluster
#ifdef VERY_SMALL
    #define N_POINTS 20
#elif SMALL
    #define N_POINTS 100
#elif MEDIUM
    #define N_POINTS 1000
#elif BIG
    #define N_POINTS 10000
#else
    #define N_POINTS 50
#endif

#define SIZE N_POINTS*N_CL //points in dataset

struct point {
    double dims[DIM];
};

void printDataset(const point*, const int*, const char*);
