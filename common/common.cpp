#include "common.h"

void printDataset(const point *X, const int *Y, const char *filename, bool end
#ifdef SCALING								
	, int sz
#endif
) {
    FILE *dataset = fopen(filename, "w");

#ifndef SCALING
    for (int i=0; i<SIZE; i++) {
#else
    for (int i=0; i<sz; i++) {
#endif
        for(int k=0; k<DIM; k++)
            fprintf(dataset, "%lg ", X[i].dims[k]);
        fprintf(dataset, end ? "%d\n" : "\n", Y[i]);
    }

    fclose(dataset);
}
