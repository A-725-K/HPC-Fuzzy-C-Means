#include "../common.h"

void mixGauss(point *means, double *sigmas, point *X, int *Y, const int NPOINTS) {
    for (int i=0; i<N_CL; i++) {
        double S = sigmas[i];
        point m = means[i];
        for (int j=0; j<NPOINTS; j++) {
            for(int k=0; k<DIM; k++){
                double r1 = ((double) rand() / (RAND_MAX));
                X[i*NPOINTS + j].dims[k] = S*r1 + m.dims[k];
            }
            Y[i*NPOINTS + j] = i;
        }
    }
}

int main(int argc, char **argv){
    srand(time(nullptr));

    point *means = new point[N_CL];
    double *sigmas = new double[N_CL];

#ifdef MEDIUM
    const char* ds_name = "medium.csv";

    means[0].dims[0] = 1;
    means[0].dims[1] = 0;

    means[1].dims[0] = 0;
    means[1].dims[1] = 1;

    means[2].dims[0] = 0;
    means[2].dims[1] = 2;

    means[3].dims[0] = 1;
    means[3].dims[1] = 3;

    means[4].dims[0] = 2;
    means[4].dims[1] = 2;

    means[5].dims[0] = 2;
    means[5].dims[1] = 1;

    sigmas[0] = 2;
    sigmas[1] = 2;
    sigmas[2] = 2;
    sigmas[3] = 2;
    sigmas[4] = 2;
    sigmas[5] = 2;
#elif BIG
    const char* ds_name = "big.csv";

    means[0].dims[0] = 0;
    means[0].dims[1] = 0;

    means[1].dims[0] = 2;
    means[1].dims[1] = 0;

    means[2].dims[0] = 4;
    means[2].dims[1] = 0;

    means[3].dims[0] = 0;
    means[3].dims[1] = 2;

    means[4].dims[0] = 2;
    means[4].dims[1] = 2;

    means[5].dims[0] = 4;
    means[5].dims[1] = 2;

    means[6].dims[0] = 0;
    means[6].dims[1] = 4;

    means[7].dims[0] = 2;
    means[7].dims[1] = 4;

    means[8].dims[0] = 4;
    means[8].dims[1] = 4;

    sigmas[0] = 3.49;
    sigmas[1] = 3.64;
    sigmas[2] = 3.89;
    sigmas[3] = 3.72;
    sigmas[4] = 3.49;
    sigmas[5] = 3.64;
    sigmas[6] = 3.89;
    sigmas[7] = 3.72;
    sigmas[8] = 3.72;
#else
    const char* ds_name = "small.csv";

    means[0].dims[0] = 0;
    means[0].dims[1] = 0;

    means[1].dims[0] = 0;
    means[1].dims[1] = 2;

    means[2].dims[0] = 2;
    means[2].dims[1] = 2;

    means[3].dims[0] = 2;
    means[3].dims[1] = 0;

    sigmas[0] = 3.49;
    sigmas[1] = 3.64;
    sigmas[2] = 3.89;
    sigmas[3] = 3.72;
#endif

		if (argc != 2 && argc != 3) {
				std::cerr << "Usage: ./generator <POINTS_PER_CL> * " << N_CL 
									<< " CLUSTERS [<DATASET_NAME>]" << std::endl;
				return -1;
		}

		if (argc == 3)
				ds_name = argv[2];
		
		const int NPOINTS = atoi(argv[1]);
		
		std::cout << "Points per cluster: " << NPOINTS << std::endl;
		std::cout << "Size of dataset   : " << NPOINTS*N_CL << std::endl;
		std::cout << "Dataset name      : " << ds_name << std::endl;

    point *X = new point[NPOINTS*N_CL];
    int *Y = new int[NPOINTS*N_CL];
    
    mixGauss(means, sigmas, X, Y, NPOINTS);
    printDataset(X, Y, ds_name, false
#ifdef SCALING
		, NPOINTS*N_CL
#endif
		);
    
    delete[] means;
    delete[] sigmas;
    delete[] X;
    delete[] Y;
    
    return 0;
}
