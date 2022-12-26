#include <R.h>
#include <Rinternals.h>

SEXP SumSquares(SEXP x){
    SEXP aux;
    PROTECT(aux = allocVector(REALSXP, 1));
    REAL(aux)[0] = 0.0;
    for(int i = 0; i < length(x); i++)
        REAL(aux)[0] += REAL(x)[i] * REAL(x)[i];
    UNPROTECT(1);
    return aux;
}