#include "fuzzy_functions.h"

int main(int argc, char **argv) {
    
	srand(time(nullptr));

    //lettura da file
    
	point *X = new point[SIZE];
	int *Y = new int[SIZE];
	
	fuzzyCMeans(X, Y);

	delete[] X;
	delete[] Y;

	return 0;
}
