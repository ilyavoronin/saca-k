#include "gtest/gtest.h"

#include "SuffixSort.h"

class SortTests : public ::testing::Test {
public:
    int test_putLMS0(std::vector <int> &data, std::vector <int> &block_begin,
                     std::vector <int> &sorted_suffixes , int &max_symb_number) {
        return SuffixSort().putLMS0(data, block_begin, sorted_suffixes, max_symb_number);
    }

    void test_inducedSort0(std::vector <int> &data,
                               std::vector <int> &block_begin,
                               std::vector <int> &sorted_suffixes,
                               int &max_symb_number,
                               bool sort_lms_strings) {
        SuffixSort().inducedSort0(data, block_begin, sorted_suffixes, max_symb_number, sort_lms_strings);
    }

    int test_formNewString0(std::vector <int> &data,
                              std::vector <int> &sorted_suffixes,
                              int lms_n) {
        SuffixSort().formNewString0(data, sorted_suffixes, lms_n);
    }

    int test_putLMS1(std::vector <int> &sorted_suffixes,
                 int beg, int size) {
        return SuffixSort().putLMS1(sorted_suffixes, beg, size);
    }
    void test_inducedSort1L(std::vector <int> &sorted_suffixes, int beg, int size, bool sort_lms_substrings) {
        SuffixSort().inducedSort1L(sorted_suffixes, beg, size, sort_lms_substrings);
    }
    const int E = SuffixSort().EMPTY;
};

//TEST 1 begin(see test_examples.txt

TEST_F(SortTests, testPutLMS0) {
    std::vector <int> data = {1, 2, 1, 3, 1, 2, 1, 5, 1, 2, 1, 8, 8, 8, 2, 2, 2, 7, 2, 3, 2, 0};
    int max_symb_number = 9;
    std::vector <int> sorted_suffixes(data.size(), -1);
    std::vector <int> block_begin = {0, 1, 7, 15, 17, 17, 18, 18, 19, 22};

    int lms_n = test_putLMS0(data, block_begin, sorted_suffixes, max_symb_number);
    std::vector <int> expected_sorted_suffixes = {21, -1, 2, 4, 6, 8, 10, -1, -1, -1, -1, -1, -1, 14, 18, -1, -1, -1, -1, -1, -1, -1};
    int expected_lms_n = 8;

    ASSERT_EQ(lms_n, expected_lms_n);
    ASSERT_EQ(sorted_suffixes, expected_sorted_suffixes);
}

TEST_F(SortTests, testInducedSort0Substrings) {
    std::vector <int> data = {1, 2, 1, 3, 1, 2, 1, 5, 1, 2, 1, 8, 8, 8, 2, 2, 2, 7, 2, 3, 2, 0};
    std::vector <int> sorted_suffixes = {21, -1, 2, 4, 6, 8, 10, -1, -1, -1, -1, -1, -1, 14, 18, -1, -1, -1, -1, -1, -1, -1};
    std::vector <int> block_begin = {0, 1, 7, 15, 17, 17, 18, 18, 19, 22};
    int max_symb_number = 9;

    test_inducedSort0(data, block_begin, sorted_suffixes, max_symb_number, true);
    std::vector <int> expected_sorted_suffixes = {21, 4, 8, 2, 6, 10, 14, 18, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};

    ASSERT_EQ(sorted_suffixes, expected_sorted_suffixes);
}

TEST_F(SortTests, testFormNewString0) {
    std::vector <int> data = {1, 2, 1, 3, 1, 2, 1, 5, 1, 2, 1, 8, 8, 8, 2, 2, 2, 7, 2, 3, 2, 0};
    std::vector <int> sorted_suffixes = {21, 4, 8, 2, 6, 10, 14, 18, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    int lms_n = 8;

    int n_distinct = test_formNewString0(data, sorted_suffixes, lms_n);
    std::vector <int> expected_sorted_suffixes = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 3, 2, 4, 2, 5, 6, 7, 0};

    ASSERT_EQ(n_distinct, 7);
    int n = sorted_suffixes.size();
    for (int i = n - 1; i > n - 1 - lms_n; i--) {
        ASSERT_EQ(sorted_suffixes[i], expected_sorted_suffixes[i]);
    }
}

TEST_F(SortTests, testPutLMS1) {
    std::vector <int> sorted_suffixes = {E, E, E, E, E, E, E, E, E, E, E, E, E, E, 3, 2, 4, 2, 5, 6, 7, 0};
    int beg = 14;
    int size = 8;

    int lms_n = test_putLMS1(sorted_suffixes, beg, size);
    std::vector <int> expected_sorted_suffixes = {21, 15, 17, E, E, E, E, E, E, E, E, E, E, E, 3, 2, 4, 2, 5, 6, 7, 0};

    ASSERT_EQ(lms_n, 3);
    ASSERT_EQ(sorted_suffixes, expected_sorted_suffixes);
}

TEST_F(SortTests, testInducedSort1LSubstrings) {
    std::vector <int> sorted_suffixes = {21, 15, 17, E, E, E, E, E, E, E, E, E, E, E, 3, 2, 4, 2, 5, 6, 7, 0};
    int beg = 14;
    int size = 8;

    test_inducedSort1L(sorted_suffixes, beg, size, true);
    std::vector <int> expected_sorted_suffixes = {E, E, E, 14, 16, E, E, 20, E, E, E, E, E, E, 3, 2, 4, 2, 5, 6, 7, 0};

    ASSERT_EQ(sorted_suffixes, expected_sorted_suffixes);
}