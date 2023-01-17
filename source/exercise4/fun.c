/*
 * Es importante notar que al trabajar con programacion
 * paralela, la manipulacion de matrices como lo hacemos
 * en C no es valida. Por esto, debemos pensar que en que
 * las columnas de los vactores se concatenan.
 *
 * Pasos:
 * 1. recibir Variables
 * 2. Calculo de Medias
 * 3. Calculo de Varianzas
 * 4. Calculo Correlaciones
 * 5. Vector resultante a Matriz.
 */

//#ifdef HAVE_LONG_DOUBLE
//# define SQRTL sqrtl
//#else
//# define SQRTL sqrt
//#endif

#include <R.h>
//#include <Rmath.h>
#include <math.h>

static void MEANS(double *x, int *nr, int *nc, double *d){

    for(int i = 0; i < *nc; i++){
        for(int j = (*nr * i); j < (*nr * i + *nr); j++){
           d[i] += x[j];
        }
        d[i] /= *nr; //luego probar dividir solo una vez
    }
}

static void VARS(double *x, int *nr, int *nc, double *d, double *xbar){

    for(int i = 0; i < *nc; i++){
        for(int j = (*nr * i); j < (*nr * i + *nr); j++){
           d[i] += (x[j] - xbar[i]) * (x[j] - xbar[i]);
        }
        d[i] /= (*nr - 1); //luego probar dividir solo una vez
    }
}

static void CORS(double *x, int *nr, int *nc, double *d, double *sigma2, double *xbar){
    int aux = 0; 
    for(int i = 0; i < *nc; i++){
        for(int j = (i + 1); j < *nc; j++){
            for(int k = 0; k < *nr; k++){
                d[aux] += (x[(*nr * i) + k] - xbar[i]) * (x[(*nr * j) + k] - xbar[j]);
            }
            //d[aux] /= (SQRTL(sigma2[i] * sigma2[j]) * (*nr - 1));
            d[aux] /= (sqrt(sigma2[i] * sigma2[j]) * (*nr - 1));
            aux++;
        }
    }
}

#include <Rinternals.h>

SEXP CoR(SEXP x, SEXP attrs){
    SEXP ans, means, vars, ind;
    int nr = nrows(x), nc = ncols(x);
    R_xlen_t N;
    N = (R_xlen_t)nc * (nc + 1)/2;
    PROTECT(ans = allocVector(REALSXP, N));
    PROTECT(means = allocVector(REALSXP, nc));
    PROTECT(vars = allocVector(REALSXP, nc));
    if(TYPEOF(x) != REALSXP)
        x = coerceVector(x, REALSXP);
    PROTECT(x);
    
    MEANS(REAL(x), &nr, &nc, REAL(means));
    VARS(REAL(x), &nr, &nc, REAL(vars), REAL(means));
    CORS(REAL(x), &nr, &nc, REAL(ans), REAL(vars), REAL(means));
    
    UNPROTECT(4);// +1
    return ans; 
}
