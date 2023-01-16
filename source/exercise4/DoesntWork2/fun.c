#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_LONG_DOUBLE
# define SQRTL sqrtl
#else
# define SQRTL sqrt
#endif

#include "Defn.h"
#include <Rmath.h>
#include <stdint.h>
#include <Rinternals.h>

#define CLAMP(X)  (X >= 1. ? 1. : (X <= -1. ? -1. : X))

#define COV_n_le_1(_n_,_k_)			        \
    if (_n_ <= 1) {/* too many missing */	\
	    for (i = 0 ; i < ncx ; i++)		    \
	        for (j = 0 ; j < _k_ ; j++)		\
		        ANS(i,j) = NA_REAL;		    \
	    return;					            \
    }


#define COV_init(_ny_)				\
    COV_ini_0;                      \
    int nobs;			            \
    /* total number of complete observations */	\
    nobs = 0;					    \
    for(k = 0 ; k < n ; k++) {		\
	    if (ind[k] != 0)            \
            nobs++;		            \
    }						        \
    COV_n_le_1(nobs, _ny_)

#define COMPLETE_1				        \
    double *z;					        \
    int i, j;					        \
    for (i = 0 ; i < n ; i++)			\
	    ind[i] = 1;				        \
    for (j = 0 ; j < ncx ; j++) {		\
	    z = &x[j * n];				    \
    	NA_LOOP					        \
    }

static void complete1(int n, int ncx, double *x, int *ind, Rboolean na_fail)
{
    COMPLETE_1
}

static void cov_complete1(int n, int ncx, double *x, double *xm,
	      int *ind, double *ans){
    
    COV_init(ncx);

    if(!kendall) {
	    MEAN(x);/* -> xm[] */
    	n1 = nobs - 1;
    }
    for (i = 0 ; i < ncx ; i++) {
	    xx = &x[i * n];

	    if(!kendall) {
	        xxm = xm[i];
	        for (j = 0 ; j <= i ; j++) {
	    	    yy = &x[j * n];
	    	    yym = xm[j];
	    	    sum = 0.;
	    	    for (k = 0 ; k < n ; k++)
	    	        if (ind[k] != 0)
	    	    	    sum += (xx[k] - xxm) * (yy[k] - yym);
	    	    ANS(j,i) = ANS(i,j) = (double)(sum / n1);
	        }
	    }
    }

    for (i = 0 ; i < ncx ; i++)
       xm[i] = sqrt(ANS(i,i));
       for (i = 0 ; i < ncx ; i++) {
           for (j = 0 ; j < i ; j++) {
       	    if (xm[i] == 0 || xm[j] == 0) {
       	        ANS(j,i) = ANS(i,j) = NA_REAL;
       	    }
       	    else {
       	        sum = ANS(i,j) / (xm[i] * xm[j]);
       	        ANS(j,i) = ANS(i,j) = (double)CLAMP(sum);
       	    }
           }
           ANS(i,i) = 1.0;
       }
} 

static SEXP cov(SEXP x, SEXP y){
    
    SEXP ans, xm, ym, ind;
    int i, method, n, ncx, ncy, nprotect = 2;
   
    x = PROTECT(coerceVector(x, REALSXP));
    if ((ansmat = isMatrix(x))){
	    n = nrows(x);
    	ncx = ncols(x);
    }
    else{
	    n = length(x);
    	ncx = 1;
    }
    

    if (isNull(y)){
        PROTECT(xm = allocVector(REALSXP, ncx));
    	PROTECT(ind = allocVector(INTSXP, n));
	    complete1(n, ncx, REAL(x), INTEGER(ind), na_fail);
    	cov_complete1(n, ncx, REAL(x), REAL(xm),
	        INTEGER(ind), REAL(ans));
	    if(empty_err) {
    		Rboolean indany = FALSE;
    		for(i = 0; i < n; i++) {
		        if(INTEGER(ind)[i] == 1) { indany = TRUE; break; }
		    }
		    if(!indany) error(_("no complete element pairs"));
	    }
	    UNPROTECT(2);
    }
    else{
    }
}
