#include <R.h>

void SumSquares(double *x, int *nx, double *aux){
    for(int i = 0; i < *nx; i++)
       *aux += x[i] * x[i];
}