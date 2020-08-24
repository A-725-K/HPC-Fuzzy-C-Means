#include "fuzzy_functions.h"

#ifdef MEDIUM
	#define RESULT "results/mpi/medium_out_MPI.csv"
#elif BIG
	#define RESULT "results/mpi/big_out_MPI.csv"
#else
	#define RESULT "results/mpi/small_out_MPI.csv"
#endif

int main(int argc, char **argv) {
	srand(time(nullptr));

	if (argc != 2) {
		fprintf(stderr, "Usage: mpirun -n {nprocs} --hostfile {hostfile} ./fuzzycm_mpi_scaling <DATASET NAME>\n");
		return -1;
	}

	const char *DATASET_NAME = argv[1];
	
	point *X = new point[SIZE];
	int *Y = new int[SIZE];

	FILE *dataset = fopen(DATASET_NAME, "r");
	for (int i=0; i<SIZE; i++) {
		for (int k=0; k<DIM; k++) {
			fscanf(dataset, "%lg ", &X[i].dims[k]);
		}
	}
	fclose(dataset);

	int P, myid;

	MPI_Init(nullptr, nullptr);
	MPI_Comm_size(MPI_COMM_WORLD, &P);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);

	auto start = MPI_Wtime();
	fuzzyCMeans(X, Y, P, myid);
	auto end = MPI_Wtime();

	if (!myid)
		std::cout << end - start << std::endl;

	MPI_Finalize();

	printDataset(X, Y, RESULT, true);

	delete[] X;
	delete[] Y;

	return 0;
}
