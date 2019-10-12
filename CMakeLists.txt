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

add_executable(mrsw_ptr mrsw_ptr_impl.cpp general_bench.cpp)
target_link_libraries(mrsw_ptr ${UTIL_LIB})
target_compile_definitions(mrsw_ptr PUBLIC MRSW_PTR)

add_executable(atomic_add_trail atomic_add_trail.cpp)
target_link_libraries(atomic_add_trail ${UTIL_LIB})

add_executable(atomic_set_trail atomic_set_trail.cpp)
target_link_libraries(atomic_set_trail ${UTIL_LIB})

find_package(Boost REQUIRED)
find_package(folly REQUIRED)
find_package(Threads REQUIRED)
find_package(gflags REQUIRED)

set_and_check(FOLLY_INCLUDE_DIR /usr/local/include/folly)
set_and_check(FOLLY_CMAKE_DIR /usr/local/lib/cmake/folly)
if (NOT TARGET Folly::folly)
    include("${FOLLY_CMAKE_DIR}/folly-targets.cmake")
endif()

add_executable(haz_ptr_impl haz_ptr_impl.cpp general_bench.cpp)
target_link_libraries(haz_ptr_impl ${UTIL_LIB} ${Boost_LIBRARIES} ${FOLLY_LIBRARIES} unwind)
target_compile_definitions(haz_ptr_impl PUBLIC HAZ_PTR)
