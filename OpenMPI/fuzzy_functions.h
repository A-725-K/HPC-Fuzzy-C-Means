#include "../common/common.h"
#include <mpi.h>

double distance(point, point);
int bestClusterIndex(double*);
double fuzzy(point, point*, int);
void initializeClustersCenters(point*, point*);
void initializeMembershipVecs(double*, point*, point*);
void adjustClustersCenters(point*, double*, point*, int, int);
double objectiveFunction(double*, point*, point*, int, int);
void fuzzyCMeans(point*, int*, int, int);
