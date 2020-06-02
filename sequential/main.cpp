#include "fuzzy_functions.h"

#ifdef MEDIUM
	#define DATASET_NAME "../common/dataset/medium.csv"
#elif BIG
	#define DATASET_NAME "../common/dataset/big.csv"
#else
	#define DATASET_NAME "../common/dataset/small.csv"
#endif

int main(int argc, char **argv) {
	srand(time(nullptr));

	point *X = new point[SIZE];
	int *Y = new int[SIZE];

	FILE *dataset = fopen(DATASET_NAME, "r");
	for (int i=0; i<SIZE; i++) {
		for (int k=0; k<DIM; k++) {
			fscanf(dataset, "%lg ", &X[i].dims[k]);
		}
	}
	fclose(dataset);

	fuzzyCMeans(X, Y);
	printDataset(X, Y, "test.csv", true);

	delete[] X;
	delete[] Y;

	return 0;
}
