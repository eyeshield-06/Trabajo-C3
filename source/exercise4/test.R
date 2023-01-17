source("fun.R")
dyn.load("fun.so")

a <- rnorm(600000)
b <- rnorm(600000)
c <- rnorm(600000)
d <- rnorm(600000)
e <- rnorm(600000)
f <- rnorm(600000)
g <- rnorm(600000)

D <- cbind(a, b, c, d, e, f, g)

system.time(cor(D))

system.time(CORS(D))
