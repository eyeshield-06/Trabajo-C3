sumsquaresv2 <- function(x){
    .Call("SumSquares",
    as.double(x))
}