sumsquares <- function(x){
    .C("SumSquares",
    as.double(x),
    as.integer(length(x)),
    aux = as.double(0))$aux
}