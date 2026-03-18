cmake_minimum_required(VERSION 3.30)
cmake_policy(SET CMP0167 NEW)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_NAME prng-benchmark)
project(${PROJECT_NAME})

find_package(Boost REQUIRED COMPONENTS random)

set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
add_subdirectory(3rdparty/benchmark)

add_executable(${PROJECT_NAME} 
                    src/benchmark.cpp
                    src/mcml_mcg59.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE include)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    benchmark::benchmark
    Boost::random
)

target_compile_options(${PROJECT_NAME} PRIVATE 
    -O3 -march=native
)