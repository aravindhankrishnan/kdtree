cmake_minimum_required (VERSION 2.8 FATAL_ERROR)

set (CMAKE_CXX_FLAGS "-std=c++11")
set (CMAKE_BUILD_TYPE DEBUG)

add_executable (test_csv_io test_csv_io.cpp
                            csv_io.cpp)

add_executable (brute_force_search brute_force_search.cpp
                            csv_io.cpp)

add_executable (build_kdtree build_kdtree.cpp
                            kdtree.cpp
                            kdtreeio.cpp
                            split_criteria.cpp
                            csv_io.cpp)

add_executable (query_kdtree query_kdtree.cpp
                            kdtree.cpp
                            kdtreeio.cpp
                            split_criteria.cpp
                            csv_io.cpp)

add_executable (generate_nd_dataset generate_nd_dataset.cpp)
