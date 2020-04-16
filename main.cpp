#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <cstring>

#define N_CL 5 		 // number of clusters
#define M 2			 // fuzzification parameter
#define DIM 2		 // dimension of the point
#define EPS 0.001	 // threshold to stop the algorithm
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

double distance(point a, point b) {
    
    double sum = 0;
    for(int k=0;k<DIM;k++){
        sum += pow(a.dims[k]-b.dims[k], 2);
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
		//int r = rand() % SIZE;
        for(int k=0;k<DIM;k++)
            cc[i].dims[k] = X[r].dims[k];
	}
}

void initializeMembershipVecs(double *mv, point *cc, point *X) {
	for (int i=0; i<SIZE; i++)
		for (int j=0; j<N_CL; j++)
			mv[i*N_CL + j] = distance(X[i], cc[j]);
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
        for(int v=0;v<DIM;v++)
            cc[k].dims[v] = sum_dims[v] / sum_den;
	}


	for (int i=0; i<N_CL; i++) {
		std::cout << "DEBUG: (";
		for (int j=0; j<DIM; j++)
			std::cout << cc[i].dims[j] << (j==DIM-1 ? "" : ", ");
		std::cout << ")" << std::endl;
	}
	std::cout << std::endl;
}

double objectiveFunction(double *mv, point *X, point *cc) {
	double J = 0.0;
	for (int i=0; i<SIZE; i++)
		for (int j=0; j<N_CL; j++)
			J += pow(mv[i*N_CL + j], M) * pow(distance(X[i], cc[j]), 2);
	std::cout << "DEBUG: J = " << J << std::endl;
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
		else
			std::cout << J << std::endl;
		old_J = J;
	}

	for (int i=0; i<SIZE; i++)
		Y[i] = bestClusterIndex(membership_vecs + i*N_CL);
	
	std::cout << n << std::endl;
	delete[] membership_vecs;
	delete[] clusters_centers;
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

int main(int argc, char **argv) {
	srand(time(nullptr));

	point *means = new point[N_CL];
	means[0].dims[0] = 0;
	means[0].dims[1] = 0;

	means[1].dims[0] = 0;
	means[1].dims[1] = 2;

	means[2].dims[0] = 2;
	means[2].dims[1] = 2;

	means[3].dims[0] = 2;
	means[3].dims[1] = 0;

	means[4].dims[0] = 1;
	means[4].dims[1] = 1;

	double *sigmas = new double[N_CL];
	sigmas[0] = 3.49;
	sigmas[1] = 3.64;
	sigmas[2] = 3.89;
	sigmas[3] = 3.72;
	sigmas[4] = 3.81;

	point *X = new point[SIZE];
	int *Y = new int[SIZE];
	
	mixGauss(means, sigmas, X, Y);
	fuzzyCMeans(X, Y);
	printDataset(X, Y, "dataset.csv");

	delete[] means;
	delete[] sigmas;
	delete[] X;
	delete[] Y;

	return 0;
}
