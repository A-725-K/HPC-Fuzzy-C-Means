#include "fuzzy_functions.h"

#ifdef MEDIUM
	#define DATASET_NAME "common/dataset/medium.csv"
	#define RESULT "results/sequential/medium_out.csv"
#elif BIG
	#define DATASET_NAME "common/dataset/big.csv"
	#define RESULT "results/sequential/big.csv"
#else
	#define DATASET_NAME "common/dataset/small.csv"
	#define RESULT "results/sequential/small_out.csv"
#endif

int main(int argc, char **argv) {
	srand(time(nullptr));

	point *X = new point[SIZE];
	int *Y = new int[SIZE];

	FILE *dataset = fopen(DATASET_NAME, "r");
	for (int i=0; i<SIZE; i++) 
		for (int k=0; k<DIM; k++) 
			fscanf(dataset, "%lg ", &X[i].dims[k]);
		
	fclose(dataset);

	const auto start = std::chrono::high_resolution_clock::now();
	fuzzyCMeans(X, Y);
	const auto end = std::chrono::high_resolution_clock::now();

	// print out the results
	printDataset(X, Y, RESULT, true);
	std::cout << std::chrono::duration_cast<std::chrono::duration<float>>(end - start).count() << std::endl;

	delete[] X;
	delete[] Y;

	return 0;
}
