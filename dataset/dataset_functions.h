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

void mixGauss(point *means, double *sigmas, point *X, int *Y) {
    for (int i=0; i<N_CL; i++) {
        double S = sigmas[i];
        point m = means[i];
        for (int j=0; j<N_POINTS; j++) {
            for(int k=0;k<DIM;k++){
                double r1 = ((double) rand() / (RAND_MAX));
                X[i*N_POINTS + j].dims[k] = S*r1 + m.dims[k];
            }
            Y[i*N_POINTS + j] = i;
        }
    }
}


void printDataset(const point *X, const int *Y, const char *filename) {
    FILE *dataset = fopen(filename, "w");

    for (int i=0; i<SIZE; i++) {
        for(int k=0;k<DIM;k++)
            fprintf(dataset, "%lg ", X[i].dims[k]);
        fprintf(dataset, "%d\n", Y[i]);
    }

    fclose(dataset);
}
