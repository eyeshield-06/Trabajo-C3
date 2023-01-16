source("fun.R")
dyn.load("fun.so")
a = as.matrix(iris[1:10, -5])

for (i in 1:10)
    a = rbind(a, a)

library(microbenchmark)
microbenchmark(cor(a), CORS(a))
