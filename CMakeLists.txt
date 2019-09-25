cmake_minimum_required(VERSION 3.12)
project(concurrent_ptr_sync)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

set(
        UTIL_LIB
        pthread
)

find_package(Boost 1.50.0 REQUIRED COMPONENTS atomic)

add_executable(no_op no_op_impl.cpp general_bench.cpp)
target_link_libraries(no_op ${UTIL_LIB})

add_executable(lock_based_no_alloc lock_based_no_allocating.cpp general_bench.cpp)
target_link_libraries(lock_based_no_alloc ${UTIL_LIB})

add_executable(lock_based lock_based_impl.cpp general_bench.cpp)
target_link_libraries(lock_based ${UTIL_LIB})

add_executable(unq_based unique_ownership_impl.cpp general_bench.cpp)
target_link_libraries(unq_based ${UTIL_LIB})

add_executable(boost_arc boost_arc_impl.cpp general_bench.cpp)
target_link_libraries(boost_arc ${UTIL_LIB})
target_compile_definitions(boost_arc PUBLIC BOOST_ARC)
target_include_directories(boost_arc PUBLIC ${Boost_INCLUDE_DIR})