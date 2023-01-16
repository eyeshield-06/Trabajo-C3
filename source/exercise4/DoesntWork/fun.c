#include <R.h>
#include <Rmath.h>

/*x matrix
n number of rows
m number of cols
out correlation matrix */

/* 
 * Problema !!
 * function .C() no acepta matrices.
 * Y se complica un poco al aceptar solo vectores, ademas la salida tampoco
 * puede ser una matriz. tendria que convertir la salida a matriz.
 * */

void cov(int *m, int *n, float *x, float *out){
    float mean_vector[*m], var_vector[*m], EXY[*m][*m];
   
    // calculation of means
    for(int i = 0; i < *m; i++){
        for(int j = 0; j < *n; j++){
            mean_vector[i] += x[j];
        }
    }

    // calculation of variances
    for(int i = 0; i < *m; i++){
        for(int j = 0; j < *n; j++){
            var_vector[i] += (*x[j, i] - mean_vector[i])*(*x[j, i] - mean_vector[i]);
        }
    }

    // calculation of E(X_i, X_j), i < j
    for(int i = 0 ; i < (*m - 1); i++){
        for(int j = i + 1; j < *m; j++){
            for(int k = 0; k < *n; k++){
                EXY[i][j] = (*x[k, i] - mean_vector[i])*(*x[k, j] - mean_vector[j]);
            }
        }
    }

    // Cor Matrix
    for(int i = 0; i < *m; i++){
        for(int j = 0; j < *m; j++){
            *out[i, j] = (EXY[i][j] - mean_vector[i]*mean_vector[j])/(sqrt(var_vector[i]*var_vector[j]));
        }
    }
}
