#include "../cuda/fuzzy_functions.h"
#include <chrono>

#ifdef MEDIUM
	#define DATASET_NAME "C:/Users/User/Desktop/HPC-Fuzzy-C-Means/common/dataset/medium.csv"
	#define RESULT "C:/Users/User/Desktop/HPC-Fuzzy-C-Means/common/python_plot/medium_cu.csv"
#elif BIG
	#define DATASET_NAME "C:/Users/User/Desktop/HPC-Fuzzy-C-Means/common/dataset/big.csv"
	#define RESULT "C:/Users/User/Desktop/HPC-Fuzzy-C-Means/common/python_plot/big_cu.csv"
#else
	#define DATASET_NAME "C:/Users/User/Desktop/HPC-Fuzzy-C-Means/common/dataset/small.csv"
	#define RESULT "C:/Users/User/Desktop/HPC-Fuzzy-C-Means/common/python_plot/small_cu.csv"
#endif

int main(int argc, char **argv) {
	std::cout << "DNAME: " << DATASET_NAME << std::endl;


	// ############ DEBUG ##############
	auto start = std::chrono::high_resolution_clock::now();

	srand(400);
	
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
	printDataset(X, Y, RESULT, true);

	delete[] X;
	delete[] Y;

	// ############ DEBUG ##############
	auto end = std::chrono::high_resolution_clock::now();
	auto dur = end - start;
	auto i_millis = std::chrono::duration_cast<std::chrono::milliseconds>(dur);
	auto f_secs = std::chrono::duration_cast<std::chrono::duration<float>>(dur);
	std::cout << "imillis:" << i_millis.count() << '\n';
	std::cout << "fsecs: " << f_secs.count() << '\n';
	
	return 0;
}
