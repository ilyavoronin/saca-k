project(saca_k_tests)

add_executable(saca_k_tests_run ../src/SuffixSort.cpp ../include/SuffixSort.h
               testSort.cpp)

target_link_libraries(saca_k_tests_run gtest gtest_main)