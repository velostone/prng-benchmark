cmake_minimum_required(VERSION 3.15)
project(PRNG_Benchmarks LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(BOOST_ROOT "C:/Environment/vcpkg/installed/x64-windows" CACHE PATH "Путь к директории Boost")
find_package(Boost 1.76 REQUIRED COMPONENTS random)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    message(STATUS "Boost найден: ${Boost_VERSION}")
endif()

set(CMAKE_PREFIX_PATH "C:/Environment/vcpkg/installed/x64-windows")
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
add_subdirectory(3rdparty/benchmark)

add_executable(prng-benchmark src/benchmark.cpp)

target_include_directories(prng-benchmark PRIVATE include)

target_link_options(prng-benchmark PRIVATE -pthread)

target_link_libraries(prng-benchmark
    PRIVATE
    Boost::random
    benchmark::benchmark
)

target_compile_options(prng-benchmark PRIVATE -O3 -march=native)