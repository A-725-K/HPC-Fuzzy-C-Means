#include "fuzzy_functions.h"

double distance(point a, point b) {
    double sum = 0;
    for(int k=0; k<DIM; k++){
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
        for(int k=0;k<DIM;k++)
            cc[i].dims[k] = X[r].dims[k];
    }
}

void adjustClustersCenters(point *cc, double *mv, point *X) {
    for (int k=0; k<N_CL; k++) {
        double sum_dims[DIM];
        for (int i=0; i<DIM; i++)
            sum_dims[i] = 0.0;

        double sum_den = 0.0;
        for (int i=0; i<SIZE; i++) {
            double u_ij_m = pow(mv[i*N_CL + k], M);
            for(int c=0; c<DIM; c++)
                sum_dims[c] += u_ij_m * X[i].dims[c];
            sum_den += u_ij_m;
        }

        for(int v=0; v<DIM; v++)
            cc[k].dims[v] = sum_dims[v] / sum_den;
    }
}

void initializeMembershipVecs(double *mv, point *cc, point *X) {
    for (int i=0; i<SIZE; i++)
        for (int j=0; j<N_CL; j++)
            mv[i*N_CL + j] = distance(X[i], cc[j]);
}

double objectiveFunction(double *mv, point *X, point *cc) {
    double J = 0.0;
    for (int i=0; i<SIZE; i++)
        for (int j=0; j<N_CL; j++)
            J += pow(mv[i*N_CL + j], M) * pow(distance(X[i], cc[j]), 2);
    return J;
}

void fuzzyCMeans(point *X, int *Y) {
    double *membership_vecs = new double[SIZE*N_CL];
    point *clusters_centers = new point[N_CL];

    memset(membership_vecs, 0, sizeof(double)*SIZE*N_CL);
    initializeClustersCenters(clusters_centers, X);
    initializeMembershipVecs(membership_vecs, clusters_centers, X);

    int n;
    double J, old_J;
    old_J = objectiveFunction(membership_vecs, X, clusters_centers);
    for (n=0; n<N_ITER; n++) {
        adjustClustersCenters(clusters_centers, membership_vecs, X);
        for (int i=0; i<SIZE; i++)
            for (int j=0; j<N_CL; j++)
                membership_vecs[i*N_CL + j] = fuzzy(X[i], clusters_centers, j);
    
        
        J = objectiveFunction(membership_vecs, X, clusters_centers);
        if (fabs(old_J - J) < EPS)
            break;
        old_J = J;
    }

    for (int i=0; i<SIZE; i++)
        Y[i] = bestClusterIndex(membership_vecs + i*N_CL);
    
    delete[] membership_vecs;
    delete[] clusters_centers;
}

