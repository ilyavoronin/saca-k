project(saca_k_tests)

add_executable(saca_k_tests_run ../src/SuffixSort.cpp
               testSort.cpp integrationTests.cpp
               ../src/inducedSortLevel1.cpp
               ../src/inducedSortLevel0.cpp
               ../src/formNewString.cpp)

target_link_libraries(saca_k_tests_run gtest gtest_main)