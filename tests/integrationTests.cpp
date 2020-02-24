#include "gtest/gtest.h"

#include <fstream>

#include "SuffixSort.h"

const int MAX_CHAR_TEST_NUM = 11;
const int MAX_BIN_TEST_NUM = 2;

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
    void load_bin_test(int test_num) {
        std::string test_file_name = PATH_PREF + "bin_tests/test" + std::to_string(test_num) + ".txt";
        std::string test_answer_file_name = PATH_PREF + "bin_tests/test" + std::to_string(test_num) + ".a";
        std::ifstream in(test_file_name, std::ios::binary);
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
    bool check_ans(std::vector <int> &suffix_array) {
        for (int i = 0; i < (int)data.size() - 1; i++) {
            int i1 = suffix_array[i];
            int i2 = suffix_array[i + 1];
            while (i1 < data.size() && i2 < data.size()) {
                if (data[i1] > data[i2]) {
                    return false;
                }
                if (data[i1] < data[i2]) {
                    break;
                }
                i1++;
                i2++;
            }
            if (i2 == data.size()) {
                return false;
            }
        }
        return true;
    }

    const std::string PATH_PREF = "../../tests/";
    std::vector <int> data;
    std::vector <int> expected_suffix_array;
};

TEST_P(IntegrationSortTests, testAll) {
    int test_number = GetParam();
    if (test_number < 100) {
        load_char_test(test_number);
    }
    if (test_number >= 100 && test_number < 200) {
        load_bin_test(test_number - 100);
    }
    std::vector <int> suffix_array;

    SuffixSort().build_suffix_array(data, suffix_array);
    ASSERT_EQ(suffix_array, expected_suffix_array);
}

INSTANTIATE_TEST_SUITE_P(CharTests, IntegrationSortTests, testing::Range(0, MAX_CHAR_TEST_NUM + 1));
INSTANTIATE_TEST_SUITE_P(BinTests, IntegrationSortTests, testing::Range(100, 100 + MAX_BIN_TEST_NUM + 1));