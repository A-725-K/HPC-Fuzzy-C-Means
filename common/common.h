#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <cstring>
#include <chrono>

#define M 2             // fuzzification parameter
#define DIM 2         // dimension of the point
#define EPS 0.001     // threshold to stop the algorithm
#define N_ITER 10000 // max number of iterations

// number of clusters and points per cluster
#ifdef SMALL
    #define N_CL 4
    #define N_POINTS 100
#elif MEDIUM
    #define N_CL 6
    #define N_POINTS 1000
#elif BIG
    #define N_CL 9
    #define N_POINTS 5000
#else
    #define N_CL 4
    #define N_POINTS 100
#endif

#define SIZE N_POINTS*N_CL //points in dataset

struct point {
    double dims[DIM];
};

void printDataset(const point*, const int*, const char*, bool);
