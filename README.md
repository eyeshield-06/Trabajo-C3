---
title: Trabajo Computacion Estadistica III
author: Rudy Miranda
date: today
---

## Table Of Contents
- [Table Of Contents](#table-of-contents)
- [Ejercicio 1](#ejercicio-1)
  - [*.C*](#c)
  - [*.Call*](#call)
  - [*.External*](#external)
- [Ejercicio 2](#ejercicio-2)
  - [Solo con *R*](#solo-con-r)
  - [Con *C*](#con-c)
    - [Metodo 1: *.C*](#metodo-1-c)
      - [fun.R](#funr)
      - [fun.c](#func)
      - [test.R](#testr)
    - [Metodo 2: *.Call*](#metodo-2-call)
      - [fun.R](#funr-1)
      - [fun.c](#func-1)
      - [test.R](#testr-1)
    - [Metodo 3: *.External*](#metodo-3-external)
      - [fun.R](#funr-2)
      - [fun.c](#func-2)
      - [test.R](#testr-2)
- [Ejercicio 3](#ejercicio-3)
  - [Implementacion de ***R***](#implementacion-de-r)
    - [Script](#script)
  - [Nuestra Implementacion](#nuestra-implementacion)
    - [Script](#script-1)
- [Ejercicio 4](#ejercicio-4)

## Ejercicio 1

### *.C*

### *.Call*

### *.External*

## Ejercicio 2

Mediante los distintos metodos obtendremos la suma de los cuadrados al cuadrado mediante un ciclo *for* con la ayuda de una variable temporal llamada *aux*.

$$
aux = \sum_{i = 0}^{n} x_i^2
$$

### Solo con *R*

```R
squares_sum <- function(v) sum(v * v)
```

### Con *C*

Todas los metodos funcionan mediante 3 archivos (reducible a 2)

1. *fun.R*, funcion *R* que llama a uno de los tres metodos y recibe los argumentos que seran pasados a la funcion de *C*.
2. *fun.C*, Ejecuta el programa y puede retornar un valor.
3. *test.R*, ejecuta un ejemplo luego de haber cargado el script de *C* con la funcion *dyn.load()* y el de *R* con *source()*.

#### Metodo 1: *.C*

##### fun.R

```R
sumsquares <- function(x){
    .C("SumSquares",
    as.double(x),
    as.integer(length(x)),
    aux = as.double(0))$aux
}
```

##### fun.c

```c
#include <R.h>

void SumSquares(double *x, int *nx, double *aux){
    for(int i = 0; i < *nx; i++)
       *aux += x[i] * x[i];
}
```

```sh
R CMD SHLIB fun.c
```
##### test.R

```R
source("fun.R")
dyn.load("fun.so")

sumsquares(1:5)
```

#### Metodo 2: *.Call*

##### fun.R

```R
sumsquaresv2 <- function(x){
    .Call("SumSquares",
    as.double(x))
}
```

##### fun.c

```c
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
```

```sh
R CMD SHLIB fun.c
```

##### test.R

```R
source("fun.R")
dyn.load("fun.so")

sumsquaresv2(1:5)
```

#### Metodo 3: *.External*

##### fun.R

```R
sumsquaresv3 <- function(...){
	.External("sumsquaresv3", ...)
}
```

##### fun.c

```c
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
```

```sh
R CMD SHLIB fun.c
```

##### test.R

```R
source("fun.R")
dyn.load("fun.so")

sumsquaresv3(1, 2, 3, 4, 5)
```
## Ejercicio 3 

El algoritmo utilizado para el calculo de la distancia euclidiana viene dado directamente por la definicion que conocemos de algebra.

$$
  d(x, y) = \sqrt{\sum_{i = 1}^{n} (x_i - y_i)^2}
$$

Notar que el $n$ corresponde a la dimension del espacio vectorial.

### Implementacion de ***R***

La funcion nativa de ***R*** para el calculo de la distancia la llamamos con `dist(x)`, donde el argumento es un vector.

Los scripts utilizador por la funcion los podemos encontrar en [Github](https://github.com/eyeshield-06/Trabajo-C3/tree/master/gitsources), ahi se encontraran dos scripts, ya que se llama a una funcion de ***C***; el repositorio con el resto de funciones del paquete base se encuentran [aqui](https://github.com/wch/r-source/tree/e5b21d0397c607883ff25cca379687b86933d730).

#### Script

```c
static double R_euclidean(double *x, int nr, int nc, int i1, int i2)
{
    double dev, dist;
    int count, j;

    count= 0;
    dist = 0;
    for(j = 0 ; j < nc ; j++) {
	    if(both_non_NA(x[i1], x[i2])) {
	      dev = (x[i1] - x[i2]); //distancia
	      if(!ISNAN(dev)) {
		      dist += dev * dev; //sum distancias al cuadrado
		      count++;
	      }
	    }
	  i1 += nr;
	  i2 += nr;
    }
    if(count == 0)
      return NA_REAL;
    if(count != nc) dist /= ((double)count/nc);
      return sqrt(dist); // raiz distancias al cuadrado
}
```

### Nuestra Implementacion

La implementacion nuestra consiste en una optimizacion de la anterior dado que solo estamos tratando el caso de la distancia euclidian, ya que la funcion  original admite 6 distancias diferentes. Por lo anterior disminuimos el numero de parametros y de funciones auxiliares, quitando con ello elementos innecesarios.

Notas las llamadas de `dist()` v/s `distancia()`

```R
dist <- function(x, method = "euclidean", diag = FALSE, upper = FALSE, p = 2)
{
    ## account for possible spellings of euclid?an
    if(!is.na(pmatch(method, "euclidian")))
	    method <- "euclidean"

    METHODS <- c("euclidean", "maximum",
		 "manhattan", "canberra", "binary", "minkowski")
    method <- pmatch(method, METHODS)
    if(is.na(method))
	    stop("invalid distance method")
    if(method == -1)
	    stop("ambiguous distance method")

    x <- as.matrix(x)
    N  <- nrow(x)
    attrs <- if(method == 6L)
        list(Size = N, Labels =  dimnames(x)[[1L]], Diag = diag,
             Upper = upper, method = METHODS[method],
             p = p, call = match.call(), class = "dist")
    else
        list(Size = N, Labels =  dimnames(x)[[1L]], Diag = diag,
             Upper = upper, method = METHODS[method],
             call = match.call(), class = "dist")
    .Call(C_Cdist, x, method, attrs, p)
}

```

```R
distancia <- function(x) {
    x <- as.matrix(x) # en el caso que x sea vector
    .Call("Cdist", x)
}
```

#### Script

Los scripts los encontraran [aqui](https://github.com/eyeshield-06/Trabajo-C3/tree/master/source/exercise3).




## Ejercicio 4