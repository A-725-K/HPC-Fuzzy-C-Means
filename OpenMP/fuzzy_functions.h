#include "../common/common.h"
#include <omp.h>

double distance(point, point);
int bestClusterIndex(double*);
double fuzzy(point, point*, int);
void initializeClustersCenters(point*, point*);
void initializeMembershipVecs(double*, point*, point*);
void adjustClustersCenters(point*, double*, point*);
double objectiveFunction(double*, point*, point*);
void fuzzyCMeans(point*, int*);
