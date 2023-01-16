distancia <- function(x) {
    x <- as.matrix(x) # en el caso que x sea vector
    n <- nrow(x) # que es lo que hace n?
    .Call("Cdist", x)
}