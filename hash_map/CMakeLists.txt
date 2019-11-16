add_executable(conc_hash_map concurrent_hash_test.cpp thread.cpp haz_ptr.cpp)
target_link_libraries(conc_hash_map ${UTIL_LIB})

#add_executable(bucket_map_bench bucket_map_bench.cpp haz_ptr.cpp)
#target_link_libraries(bucket_map_bench ${UTIL_LIB})

#add_executable(haz_debug_1 haz_debug_1.cpp haz_ptr.cpp)
#target_link_libraries(haz_debug_1 ${UTIL_LIB})
#target_compile_definitions(haz_debug_1 PUBLIC HAZ_PTR_DEBUG)

add_executable(hash_map_bench hash_map_bench.cpp thread.cpp haz_ptr.cpp)
target_link_libraries(hash_map_bench ${UTIL_LIB})

add_executable(ft_emulate fast_table_emulate.cpp thread.cpp haz_ptr.cpp)
target_link_libraries(ft_emulate ${UTIL_LIB})

add_executable(zip_fan_test zip_fan_test.cpp)

add_executable(fast_table fast_table_test.cpp thread.cpp haz_ptr.cpp)
target_link_libraries(fast_table ${UTIL_LIB})

add_executable(ss_debug ss_debug.cpp thread.cpp haz_ptr.cpp)
target_link_libraries(ss_debug ${UTIL_LIB})

add_executable(hit_ratio hit_ratio.cpp)
