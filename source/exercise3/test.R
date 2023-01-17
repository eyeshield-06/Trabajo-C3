source("fun.R")
dyn.load("fun.so")

a <- rnorm(20000)
b <- rnorm(20000)

c <- cbind(a, b)

system.time(dist(c))

system.time(distancia(c))
