#include "../common/common.h"
#include <cuda.h>
#include <cuda_runtime.h>

__host__ __device__ double distance(point, point);
int bestClusterIndex(double*);
double fuzzy(point, point*, int);
void initializeClustersCenters(point*, point*);
void initializeMembershipVecs(double*, point*, point*);
__global__ void objectiveFunction(double*, point*, point*, double*);
void fuzzyCMeans(point*, int*);
void adjustClustersCenters(point*, double*, point*);