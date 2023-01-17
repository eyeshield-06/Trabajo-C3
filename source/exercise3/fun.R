distancia <- function(x) {
    x <- as.matrix(x) # en el caso que x sea vector
    attrs <- list(Size = nrow(x), Labels =  dimnames(x)[[1L]], Diag = FALSE,
             Upper = FALSE, class = 'dist')

    .Call("Cdist", x, attrs)
}
