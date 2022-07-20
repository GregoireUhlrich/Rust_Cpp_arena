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
    threads.h
    threads.cpp
    algorithm.h
    algorithm.cpp
)

add_library(benchmark-obj OBJECT ${BENCHMARK_SRC})
target_link_libraries(benchmark-obj pthread)
