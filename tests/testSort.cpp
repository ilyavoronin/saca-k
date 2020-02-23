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

    void test_putSortedLMS1(std::vector<int> &suffix_array, int lms_n) {
        SuffixSort().putSortedLMS1(suffix_array, lms_n);
    }

    void test_putLMSToBegin0(std::vector <int> &data, std::vector <int> &suffix_array, int lms_n) {
        SuffixSort().putLMSToBegin0(data, suffix_array, lms_n);
    }

    void test_putSortedLMS0(std::vector <int> &data,
                            std::vector <int> &block_begin,
                            std::vector <int> &suffix_array,
                            int max_symb_number, int lms_n) {
        SuffixSort().putSortedLMS0(data, block_begin, suffix_array, max_symb_number, lms_n);
    }

    const int E = SuffixSort().EMPTY;
};

//TEST 1 begin(see test_examples.txt

TEST_F(SortTests, testPutLMS0) {
    std::vector <int> data = {1, 2, 1, 3, 1, 2, 1, 5, 1, 2, 1, 8, 8, 8, 2, 2, 2, 7, 2, 3, 2, 0};
    int max_symb_number = 9;
    std::vector <int> suffix_array(data.size(), E);
    std::vector <int> block_begin = {0, 1, 7, 15, 17, 17, 18, 18, 19, 22};

    int lms_n = test_putLMS0(data, block_begin, suffix_array, max_symb_number);
    std::vector <int> expected_suffix_array = {21, E, 2, 4, 6, 8, 10, E, E, E, E, E, E, 14, 18, E, E, E, E, E, E, E};
    int expected_lms_n = 8;

    ASSERT_EQ(lms_n, expected_lms_n);
    ASSERT_EQ(suffix_array, expected_suffix_array);
}

TEST_F(SortTests, testInducedSort0Substrings) {
    std::vector <int> data = {1, 2, 1, 3, 1, 2, 1, 5, 1, 2, 1, 8, 8, 8, 2, 2, 2, 7, 2, 3, 2, 0};
    std::vector <int> suffix_array = {21, E, 2, 4, 6, 8, 10, E, E, E, E, E, E, 14, 18, E, E, E, E, E, E, E};
    std::vector <int> block_begin = {0, 1, 7, 15, 17, 17, 18, 18, 19, 22};
    int max_symb_number = 9;

    test_inducedSort0(data, block_begin, suffix_array, max_symb_number, true);
    std::vector <int> expected_suffix_array = {21, 4, 8, 2, 6, 10, 14, 18, E, E, E, E, E, E, E, E, E, E, E, E, E, E};

    ASSERT_EQ(suffix_array, expected_suffix_array);
}

TEST_F(SortTests, testFormNewString0) {
    std::vector <int> data = {1, 2, 1, 3, 1, 2, 1, 5, 1, 2, 1, 8, 8, 8, 2, 2, 2, 7, 2, 3, 2, 0};
    std::vector <int> suffix_array = {21, 4, 8, 2, 6, 10, 14, 18, E, E, E, E, E, E, E, E, E, E, E, E, E, E};
    int lms_n = 8;

    int n_distinct = test_formNewString0(data, suffix_array, lms_n);
    std::vector <int> expected_suffix_array = {E, E, E, E, E, E, E, E, E, E, E, E, E, E, 3, 2, 4, 2, 5, 6, 7, 0};

    ASSERT_EQ(n_distinct, 7);
    ASSERT_EQ(suffix_array, expected_suffix_array);
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

TEST_F(SortTests, testPutSortedLMS1) {
    std::vector <int> suffix_array = {21, 15, 17, E, E, E, E, E, E, E, E, E, E, E, 3, 2, 4, 2, 5, 6, 7, 0};
    int lms_n = 3;

    test_putSortedLMS1(suffix_array, lms_n);
    std::vector <int> expected_suffix_array = {21, 15, 17, E, E, E, E, E, E, E, E, E, E, E, 3, 2, 4, 2, 5, 6, 7, 0};

    ASSERT_EQ(suffix_array, expected_suffix_array);
}

TEST_F(SortTests, testInduceSortL) {
    std::vector <int> suffix_array = {21, 15, 17, E, E, E, E, E, E, E, E, E, E, E, 3, 2, 4, 2, 5, 6, 7, 0};
    int beg = 14;
    int size = 8;

    test_inducedSort1L(suffix_array, beg, size, false);
    std::vector <int> expected_suffix_array = {E, E, E, 14, 16, E, E, 20, E, E, E, E, E, E, 3, 2, 4, 2, 5, 6, 7, 0};

    ASSERT_EQ(suffix_array, expected_suffix_array);
}

TEST_F(SortTests, testInduceSortS) {
    std::vector <int> suffix_array = {E, E, E, 14, 16, E, E, 20, E, E, E, E, E, E, 3, 2, 4, 2, 5, 6, 7, 0};
    int beg = 14;
    int size = 8;

    test_inducedSort1S(suffix_array, beg, size, false);
    std::vector <int> expected_suffix_array = {21, 15, 17, 14, 16, 18, 19, 20, E, E, E, E, E, E, 3, 2, 4, 2, 5, 6, 7, 0};

    ASSERT_EQ(suffix_array, expected_suffix_array);
}

TEST_F(SortTests, testPutLMSTobegin0) {
    std::vector <int> data = {1, 2, 1, 3, 1, 2, 1, 5, 1, 2, 1, 8, 8, 8, 2, 2, 2, 7, 2, 3, 2, 0};
    std::vector <int> suffix_array = {21, 15, 17, 14, 16, 18, 19, 20, E, E, E, E, E, E, 3, 2, 4, 2, 5, 6, 7, 0};
    int lms_n = 8;

    test_putLMSToBegin0(data, suffix_array, lms_n);
    std::vector <int> expected_suffix_array = {21, 4, 8, 2, 6, 10, 14, 18, E, E, E, E, E, E, E, E, E, E, E, E, E, E};

    ASSERT_EQ(suffix_array, expected_suffix_array);
}

TEST_F(SortTests, testPutSortedLMS0) {
    std::vector <int> data = {1, 2, 1, 3, 1, 2, 1, 5, 1, 2, 1, 8, 8, 8, 2, 2, 2, 7, 2, 3, 2, 0};
    std::vector <int> block_begin = {0, 1, 7, 15, 17, 17, 18, 18, 19, 22};
    std::vector <int> suffix_array = {21, 4, 8, 2, 6, 10, 14, 18, E, E, E, E, E, E, E, E, E, E, E, E, E, E};
    int max_symb_number = 9;
    int lms_n = 8;

    test_putSortedLMS0(data, block_begin, suffix_array, max_symb_number, lms_n);
    std::vector <int> expected_suffix_array = {21, E, 4, 8, 2, 6, 10, E, E, E, E, E, E, 14, 18, E, E, E, E, E, E, E};

    ASSERT_EQ(suffix_array, expected_suffix_array);
}

TEST_F(SortTests, testInducedSort0) {
    std::vector <int> data = {1, 2, 1, 3, 1, 2, 1, 5, 1, 2, 1, 8, 8, 8, 2, 2, 2, 7, 2, 3, 2, 0};
    std::vector <int> block_begin = {0, 1, 7, 15, 17, 17, 18, 18, 19, 22};
    std::vector <int> suffix_array = {21, E, 4, 8, 2, 6, 10, E, E, E, E, E, E, 14, 18, E, E, E, E, E, E, E};
    int max_symb_number = 9;

    test_inducedSort0(data, block_begin, suffix_array, max_symb_number, false);
    std::vector <int> expected_suffix_array = {21, 0, 4, 8, 2, 6, 10, 20, 1, 5, 9, 14, 15, 18, 16, 3, 19, 7, 17, 13, 12, 11};

    ASSERT_EQ(suffix_array, expected_suffix_array);
}

TEST_F(SortTests, testAll) {
    std::vector <int> data = {0, 1, 0, 2, 0, 1, 0, 4, 0, 1, 0, 7, 7, 7, 1, 1, 1, 6, 1, 2, 1};
    std::vector <int> suffix_array;

    SuffixSort().build_suffix_array(data, suffix_array);
    std::vector <int> expected_suffix_array = {0, 4, 8, 2, 6, 10, 20, 1, 5, 9, 14, 15, 18, 16, 3, 19, 7, 17, 13, 12, 11};

    ASSERT_EQ(suffix_array, expected_suffix_array);
}