#include "gtest/gtest.h"

#include <fstream>

#include "SuffixSort.h"
#include <windows.h>

const int MAX_CHAR_TEST_NUM = 2;

class IntegrationSortTests : public testing::TestWithParam<int> {
public:
    void load_char_test(int test_num) {
        std::string test_file_name = PATH_PREF + "char_tests/test" + std::to_string(test_num) + ".txt";
        std::string test_answer_file_name = PATH_PREF + "char_tests/test" + std::to_string(test_num) + ".a";
        std::ifstream in(test_file_name);
        char c;
        while (in.get(c)) {
            data.push_back(((int)c + 256) % 256);
        }
        in.close();
        in.open(test_answer_file_name);
        int n;
        while (in >> n) {
            expected_suffix_array.push_back(n);
        }
    }

    const std::string PATH_PREF = "../../tests/";
    std::vector <int> data;
    std::vector <int> expected_suffix_array;
};

TEST_P(IntegrationSortTests, testAll) {
    int test_number = GetParam();
    load_char_test(test_number);
    std::vector <int> suffix_array;

    SuffixSort().build_suffix_array(data, suffix_array);
    ASSERT_EQ(suffix_array, expected_suffix_array);
}

INSTANTIATE_TEST_SUITE_P(CharTests, IntegrationSortTests, testing::Range(1, MAX_CHAR_TEST_NUM + 1));