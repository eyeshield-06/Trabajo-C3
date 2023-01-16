CoV <- function(x) {
    DIM = dim(x)
    if(DIM[1] == DIM[2])
        stop("Error en Dimensiones")
    else{
        .C("cov", as.integer(DIM[1]), as.integer(DIM[2]),
           as.double(x), out = double(DIM[2]^2))
    }
}
