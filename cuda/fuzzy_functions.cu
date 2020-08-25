#include "../cuda/fuzzy_functions.h"

#include <cuda.h>
#include <cuda_runtime.h>
#include <curand_kernel.h>
#include "device_launch_parameters.h"
#include <chrono>


#define cudaCheckErrors(msg) \
    do { \
        cudaError_t __err = cudaGetLastError(); \
        if (__err != cudaSuccess) { \
            fprintf(stderr, "Fatal error: %s (%s at %s:%d)\n", \
                msg, cudaGetErrorString(__err), \
                __FILE__, __LINE__); \
            fprintf(stderr, "*** FAILED - ABORTING\n"); \
            exit(1); \
        } \
    } while (0)


__host__ __device__ double distance(point a, point b) {
    double sum = 0;
    for (int k = 0; k < DIM; k++) {
        sum += pow(a.dims[k] - b.dims[k], 2);
    }

    return sqrt(sum);
}

int bestClusterIndex(double *v) {
    int idxMax = 0;
    for (int i=1; i<N_CL; i++)
        if (v[i] > v[idxMax])
            idxMax = i;
    return idxMax;
}

double fuzzy(point x, point *clusters_centers, int clusterIdx) {
    double sum = 0.0;
    for (int i=0; i<N_CL; i++) {
        double frac = distance(x, clusters_centers[clusterIdx]) / distance(x, clusters_centers[i]);
        sum += pow(frac, 2/(M-1));
    }
    return 1/sum;
}

void initializeClustersCenters(point *cc, point *X) {
    for (int i=0; i<N_CL; i++) {
        int r = (rand() % N_POINTS) + i*N_POINTS;
        for(int k=0; k<DIM; k++)
            cc[i].dims[k] = X[r].dims[k];
    }
}

void adjustClustersCenters(point* cc, double* mv, point* X) {
    for (int k = 0; k < N_CL; k++) {
        double sum_dims[DIM];
        for (int i = 0; i < DIM; i++)
            sum_dims[i] = 0.0;

        double sum_den = 0.0;
        for (int i = 0; i < SIZE; i++) {
            double u_ij_m = pow(mv[i * N_CL + k], M);
            for (int c = 0; c < DIM; c++)
                sum_dims[c] += u_ij_m * X[i].dims[c];
            sum_den += u_ij_m;
        }

        for (int v = 0; v < DIM; v++)
            cc[k].dims[v] = sum_dims[v] / sum_den;
    }
}

void initializeMembershipVecs(double *mv, point *cc, point *X) {
    for (int i=0; i<SIZE; i++)
        for (int j=0; j<N_CL; j++)
            mv[i*N_CL + j] = distance(X[i], cc[j]);
}

__global__ void objectiveFunction(double *mv, point *X, point *cc, double *J) {
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if (row < SIZE && col < N_CL)
        *J += pow(mv[row*N_CL + col], M) * pow(distance(X[row], cc[col]), 2);
}

void fuzzyCMeans(point *X, int *Y) {
    double *membership_vecs = new double[SIZE*N_CL];
    point* clusters_centers = new point[N_CL];

    int n;
    double J, old_J = 0.0;

    initializeClustersCenters(clusters_centers, X);
    initializeMembershipVecs(membership_vecs, clusters_centers, X);

    const int THREADS = 32;
    const dim3 threads(THREADS, THREADS);
    const int BLOCKS = ((SIZE + threads.x - 1) / threads.x);
    const dim3 blocks(BLOCKS, BLOCKS);

    point *GPU_X, *GPU_cluster_centers;
    double* GPU_membership_vecs, *GPU_oldJ, *GPU_J;

    size_t dimX = SIZE * sizeof(point);
    size_t dimCC = N_CL * sizeof(point);
    size_t dimMV = SIZE * N_CL * sizeof(double);
    size_t dimD = sizeof(double);

    cudaMalloc(&GPU_X, dimX);
    cudaMalloc(&GPU_cluster_centers, dimCC);
    cudaMalloc(&GPU_membership_vecs, dimMV);
    cudaMalloc(&GPU_J, dimD);
    cudaMalloc(&GPU_oldJ, dimD);

    cudaMemcpy(GPU_X, X, dimX, cudaMemcpyHostToDevice);
    cudaMemcpy(GPU_cluster_centers, clusters_centers, dimCC, cudaMemcpyHostToDevice);
    cudaMemcpy(GPU_membership_vecs, membership_vecs, dimMV, cudaMemcpyHostToDevice);
    cudaMemcpy(GPU_J, &J, dimD, cudaMemcpyHostToDevice);
    cudaMemcpy(GPU_oldJ, &old_J, dimD, cudaMemcpyHostToDevice);

    objectiveFunction<<< blocks, threads >>>(GPU_membership_vecs, GPU_X, GPU_cluster_centers, GPU_oldJ);

    cudaDeviceSynchronize();
    cudaMemcpy(&old_J, GPU_oldJ, dimD, cudaMemcpyDeviceToHost);

    for (n=0; n<N_ITER; n++) {
        adjustClustersCenters(clusters_centers, membership_vecs, X);

        for (int i=0; i<SIZE; i++)
            for (int j=0; j<N_CL; j++)
                membership_vecs[i*N_CL + j] = fuzzy(X[i], clusters_centers, j);
    
        cudaMemcpy(GPU_cluster_centers, clusters_centers, dimCC, cudaMemcpyHostToDevice);
        cudaMemcpy(GPU_membership_vecs, membership_vecs, dimMV, cudaMemcpyHostToDevice);

        objectiveFunction<<< blocks, threads >>>(GPU_membership_vecs, GPU_X, GPU_cluster_centers, GPU_J);
        
        cudaDeviceSynchronize();
        cudaMemcpy(&J, GPU_J, dimD, cudaMemcpyDeviceToHost);

        if (fabs(old_J - J) < EPS)
            break;
        old_J = J;
    }

    for (int i=0; i<SIZE; i++)
        Y[i] = bestClusterIndex(membership_vecs + i*N_CL);
    
    std::cout << "n:" << n << std::endl;

    cudaFree(GPU_X);
    cudaFree(GPU_cluster_centers);
    cudaFree(GPU_membership_vecs);
    cudaFree(GPU_J);
    cudaFree(GPU_oldJ);

    delete[] membership_vecs;
    delete[] clusters_centers;
}

