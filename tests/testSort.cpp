#include "gtest/gtest.h"

#include "SuffixSort.h"

class SortTests : public ::testing::Test {
public:
    int test_putLMS0(std::vector <int> &data, std::vector <int> &block_begin,
                     std::vector <int> &suffix_array , int &max_symb_number) {
        return SuffixSort().putLMS0(data, block_begin, suffix_array, max_symb_number);
    }

    void test_inducedSort0(std::vector <int> &data,
                               std::vector <int> &block_begin,
                               std::vector <int> &suffix_array,
                               int &max_symb_number,
                               bool sort_lms_strings) {
        SuffixSort().inducedSort0(data, block_begin, suffix_array, max_symb_number, sort_lms_strings);
    }

    int test_formNewString0(std::vector <int> &data,
                              std::vector <int> &suffix_array,
                              int lms_n) {
        SuffixSort().formNewString0(data, suffix_array, lms_n);
    }

    int test_putLMS1(std::vector <int> &suffix_array,
                 int beg, int size) {
        return SuffixSort().putLMS1(suffix_array, beg, size);
    }

    void test_inducedSort1L(std::vector <int> &suffix_array, int beg, int size, bool sort_lms_substrings) {
        SuffixSort().inducedSort1L(suffix_array, beg, size, sort_lms_substrings);
    }

    void test_inducedSort1S(std::vector <int> &suffix_array, int beg, int size, bool sort_lms_substrings) {
        SuffixSort().inducedSort1S(suffix_array, beg, size, sort_lms_substrings);
    }

    int test_formNewString1(std::vector <int> &suffix_array, int lms_n, int begin, int size) {
        return SuffixSort().formNewString1(suffix_array, lms_n, begin, size);
    }

    void test_putLMSToBegin1(std::vector <int> &suffix_array, int lms_n, int beg, int size) {
        SuffixSort().putLMSToBegin1(suffix_array, lms_n, beg, size);
    }

    const int E = SuffixSort().EMPTY;
};

//TEST 1 begin(see test_examples.txt

TEST_F(SortTests, testPutLMS0) {
    std::vector <int> data = {1, 2, 1, 3, 1, 2, 1, 5, 1, 2, 1, 8, 8, 8, 2, 2, 2, 7, 2, 3, 2, 0};
    int max_symb_number = 9;
    std::vector <int> suffix_array(data.size(), -1);
    std::vector <int> block_begin = {0, 1, 7, 15, 17, 17, 18, 18, 19, 22};

    int lms_n = test_putLMS0(data, block_begin, suffix_array, max_symb_number);
    std::vector <int> expected_suffix_array = {21, -1, 2, 4, 6, 8, 10, -1, -1, -1, -1, -1, -1, 14, 18, -1, -1, -1, -1, -1, -1, -1};
    int expected_lms_n = 8;

    ASSERT_EQ(lms_n, expected_lms_n);
    ASSERT_EQ(suffix_array, expected_suffix_array);
}

TEST_F(SortTests, testInducedSort0Substrings) {
    std::vector <int> data = {1, 2, 1, 3, 1, 2, 1, 5, 1, 2, 1, 8, 8, 8, 2, 2, 2, 7, 2, 3, 2, 0};
    std::vector <int> suffix_array = {21, -1, 2, 4, 6, 8, 10, -1, -1, -1, -1, -1, -1, 14, 18, -1, -1, -1, -1, -1, -1, -1};
    std::vector <int> block_begin = {0, 1, 7, 15, 17, 17, 18, 18, 19, 22};
    int max_symb_number = 9;

    test_inducedSort0(data, block_begin, suffix_array, max_symb_number, true);
    std::vector <int> expected_suffix_array = {21, 4, 8, 2, 6, 10, 14, 18, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};

    ASSERT_EQ(suffix_array, expected_suffix_array);
}

TEST_F(SortTests, testFormNewString0) {
    std::vector <int> data = {1, 2, 1, 3, 1, 2, 1, 5, 1, 2, 1, 8, 8, 8, 2, 2, 2, 7, 2, 3, 2, 0};
    std::vector <int> suffix_array = {21, 4, 8, 2, 6, 10, 14, 18, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    int lms_n = 8;

    int n_distinct = test_formNewString0(data, suffix_array, lms_n);
    std::vector <int> expected_suffix_array = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 3, 2, 4, 2, 5, 6, 7, 0};

    ASSERT_EQ(n_distinct, 7);
    int n = suffix_array.size();
    for (int i = n - 1; i > n - 1 - lms_n; i--) {
        ASSERT_EQ(suffix_array[i], expected_suffix_array[i]);
    }
}

TEST_F(SortTests, testPutLMS1) {
    std::vector <int> suffix_array = {E, E, E, E, E, E, E, E, E, E, E, E, E, E, 3, 2, 4, 2, 5, 6, 7, 0};
    int beg = 14;
    int size = 8;

    int lms_n = test_putLMS1(suffix_array, beg, size);
    std::vector <int> expected_suffix_array = {21, 15, 17, E, E, E, E, E, E, E, E, E, E, E, 3, 2, 4, 2, 5, 6, 7, 0};

    ASSERT_EQ(lms_n, 3);
    ASSERT_EQ(suffix_array, expected_suffix_array);
}

TEST_F(SortTests, testInducedSort1LSubstrings) {
    std::vector <int> suffix_array = {21, 15, 17, E, E, E, E, E, E, E, E, E, E, E, 3, 2, 4, 2, 5, 6, 7, 0};
    int beg = 14;
    int size = 8;

    test_inducedSort1L(suffix_array, beg, size, true);
    std::vector <int> expected_suffix_array = {E, E, E, 14, 16, E, E, 20, E, E, E, E, E, E, 3, 2, 4, 2, 5, 6, 7, 0};

    ASSERT_EQ(suffix_array, expected_suffix_array);
}

TEST_F(SortTests, testInducedSort1SSubstrings) {
    std::vector <int> suffix_array = {E, E, E, 14, 16, E, E, 20, E, E, E, E, E, E, 3, 2, 4, 2, 5, 6, 7, 0};
    int beg = 14;
    int size = 8;

    test_inducedSort1S(suffix_array, beg, size, true);
    std::vector <int> expected_suffix_array = {21, 15, 17, E, E, E, E, E, E, E, E, E, E, E, 3, 2, 4, 2, 5, 6, 7, 0};

    ASSERT_EQ(suffix_array, expected_suffix_array);
}

TEST_F(SortTests, testFormNewString1) {
    std::vector <int> suffix_array = {21, 15, 17, E, E, E, E, E, E, E, E, E, E, E, 3, 2, 4, 2, 5, 6, 7, 0};
    int beg = 14;
    int size = 8;
    int lms_n = 3;

    int n_distinct = test_formNewString1(suffix_array, lms_n, beg, size);
    std::vector <int> expected_suffix_array = {E, E, E, E, E, E, E, E, E, E, E, 1, 2, 0, 3, 2, 4, 2, 5, 6, 7, 0};

    ASSERT_EQ(n_distinct, 3);
    ASSERT_EQ(suffix_array, expected_suffix_array);
}

TEST_F(SortTests, testPutLMSToBegin1) {
    std::vector <int> suffix_array = {13, 11, 12, E, E, E, E, E, E, E, E, 1, 2, 0, 3, 2, 4, 2, 5, 6, 7, 0};
    int beg = 14;
    int size = 8;
    int lms_n = 3;

    test_putLMSToBegin1(suffix_array, lms_n, beg, size);
    std::vector <int> expected_suffix_array = {21, 15, 17, E, E, E, E, E, E, E, E, E, E, E, 3, 2, 4, 2, 5, 6, 7, 0};

    ASSERT_EQ(suffix_array, expected_suffix_array);
}