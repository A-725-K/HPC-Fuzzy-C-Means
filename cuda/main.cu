#include "../cuda/fuzzy_functions.h"

#ifdef MEDIUM
	#define DATASET_NAME "C:/Users/User/Desktop/HPC-Fuzzy-C-Means/common/dataset/medium.csv"
	#define RESULT "C:/Users/User/Desktop/HPC-Fuzzy-C-Means/results/cuda/medium_cu.csv"
#elif BIG
	#define DATASET_NAME "C:/Users/User/Desktop/HPC-Fuzzy-C-Means/common/dataset/big.csv"
	#define RESULT "C:/Users/User/Desktop/HPC-Fuzzy-C-Means/results/cuda/big_cu.csv"
#else
	#define DATASET_NAME "C:/Users/User/Desktop/HPC-Fuzzy-C-Means/common/dataset/small.csv"
	#define RESULT "C:/Users/User/Desktop/HPC-Fuzzy-C-Means/results/cuda/small_cu.csv"
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

	auto start = std::chrono::high_resolution_clock::now();
	fuzzyCMeans(X, Y);
	auto end = std::chrono::high_resolution_clock::now();
	
	printDataset(X, Y, RESULT, true);
	std::cout << std::chrono::duration_cast<std::chrono::duration<float>>(end - start).count() << std::endl;

	delete[] X;
	delete[] Y;

	return 0;
}
