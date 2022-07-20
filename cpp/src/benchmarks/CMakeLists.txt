set(BENCHMARK_SRC
    config.h
    config.cpp
    dictionnary.h
    dictionnary.cpp
    algebra.h
    algebra.cpp
    vector.h
    vector.cpp
    array.h
    array.cpp
)

add_library(benchmark-obj OBJECT ${BENCHMARK_SRC})
