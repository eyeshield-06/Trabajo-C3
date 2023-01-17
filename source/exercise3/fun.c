#include <R.h>
#include <Rmath.h>

#define both_FINITE(a,b) (R_FINITE(a) && R_FINITE(b))
#ifdef R_160_and_older
#define both_non_NA both_FINITE
#else
#define both_non_NA(a,b) (!ISNAN(a) && !ISNAN(b))
#endif

static double R_euclidean(double *x, int nr, int nc, int i1, int i2){
    double dev, dist = 0;
    int count = 0;

    for(int j = 0; j < nc; j++){
        if(both_non_NA(x[i1], x[i2])) {
            dev = x[i1] - x[i2];
            if(!ISNAN(dev)){
                dist += dev * dev;
                count++;
            }
        }
        i1 += nr;
        i2 += nr;
    }
    if(count == 0) return NA_REAL;
    if(count != nc) dist /= ((double)count/nc);
    return sqrt(dist);
}

void R_distance(double *x, int *nr, int *nc, double *d, int *diag){
    int dc, i, j;
    dc = (*diag) ? 0 : 1;
    size_t ij;
    ij = 0;
    for(j = 0; j <= *nr; j++){
        for(i = j + dc; i < *nr; i++){
            d[ij++] = R_euclidean(x, *nr, *nc, i, j);
        }
    }
}

#include <Rinternals.h>

SEXP Cdist(SEXP x, SEXP attrs){
    SEXP ans;
    int nr = nrows(x), nc = ncols(x);
    int diag = 0;
    R_xlen_t N;
    N = (R_xlen_t)nr * (nr-1)/2; // suma primeros n-numeros
    PROTECT(ans = allocVector(REALSXP, N));
    if(TYPEOF(x) != REALSXP)
        x = coerceVector(x, REALSXP);
    PROTECT(x);
    R_distance(REAL(x), &nr, &nc, REAL(ans), &diag);

    // to matrix
    SEXP names = getAttrib(attrs, R_NamesSymbol);
    for (int i = 0; i < LENGTH(attrs); i++)
	setAttrib(ans, install(translateChar(STRING_ELT(names, i))),
		  VECTOR_ELT(attrs, i));

    UNPROTECT(2);
    return ans;
}

