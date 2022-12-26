#include <R.h>
#include <Rinternals.h>

SEXP sumsquaresv3(SEXP x){
    SEXP aux, now;
    PROTECT(aux = allocVector(REALSXP, 1));
    REAL(aux)[0] = 0.0;
    x = CDR(x);
    now = CAR(x);
    while(x != R_NilValue){
        x = CDR(x);
        REAL(aux)[0] += REAL(now)[0] * REAL(now)[0];
        now = CAR(x);
    }
    UNPROTECT(1);
    return aux;
}