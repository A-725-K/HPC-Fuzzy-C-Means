#include "fuzzy_functions.h"

int main(int argc, char **argv) {
	srand(time(nullptr));

	point *X = new point[SIZE];
	int *Y = new int[SIZE];

	FILE *dataset = fopen("dataset.csv", "r");
	for (int i=0; i<SIZE; i++) {
		for (int k=0; k<DIM; k++) {
			fscanf(dataset, "%lg ", &X[i].dims[k]);
		}
	}
	fclose(dataset);
	printDataset(X, Y, "test.csv");

	//fuzzyCMeans(X, Y);

	delete[] X;
	delete[] Y;

	return 0;
}
