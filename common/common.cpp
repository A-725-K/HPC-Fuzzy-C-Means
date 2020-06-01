#include "common.h"

void printDataset(const point *X, const int *Y, const char *filename) {
    FILE *dataset = fopen(filename, "w");

    for (int i=0; i<SIZE; i++) {
        for(int k=0; k<DIM; k++)
            fprintf(dataset, "%lg ", X[i].dims[k]);
        fprintf(dataset, "%d\n", Y[i]);
    }

    fclose(dataset);
}
