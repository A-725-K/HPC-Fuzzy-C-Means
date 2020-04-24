#include "fuzzy_functions.h"

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
