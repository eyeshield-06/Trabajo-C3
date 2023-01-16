CoV <- function(x){
    if(length(x) != length(y))
        stop("Dimensiones No Validas")
    else
        .Call("cov", x, y)
}
