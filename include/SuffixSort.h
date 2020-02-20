#ifndef SACA_K_SUFFIXSORT_H
#define SACA_K_SUFFIXSORT_H

#include <vector>
#include <climits>

class SuffixSort {
public:
    void sort(std::vector <int> &data, std::vector <int> &sorted_suffixes);

private:
    //first level of recursion, suffix array will be stored in `sorted_suffixes`
    void sortSuffixes0(std::vector <int> &data, std::vector <int> &sorted_suffixes);

    //array is stored in `sorted_suffixes` from `beg` to `beg + size`
    //resulting suffix array will be stored in the beginning og the `sorted_suffixes`
    void sortSuffixes1(std::vector <int> &sorted_suffixes, int beg, int size);

    //put LMS suffixes that hasn't been sorted in their blocks(level 0)
    //return number of lms suffixes
    int putLMS0(std::vector <int> &data,
                              std::vector <int> &block_begin,
                              std::vector <int> &sorted_suffixes,
                              int max_symb_number);

    //put sorted LMS suffixes in their blocks(level 0)
    void putSortedLMS0(std::vector <int> &data,
                 std::vector <int> &block_begin,
                 std::vector <int> &sorted_suffixes,
                 int max_symb_number, int lms_n);

    //sort L and S suffixes if lms-suffixes had already been stored in their blocks(level 0)
    //when we store lms-substrings we don't need to store all lms-prefixes,
    //sort_lms_strings corresponds for that
    void inducedSort0(std::vector <int> &data,
                     std::vector <int> &block_begin,
                     std::vector <int> &sorted_suffixes,
                     int max_symb_number,
                     bool sort_lms_strings);

    //return the number of distinct values in new string
    int formNewString0(std::vector <int> &data,
                       std::vector <int> &sorted_suffixes,
                       int lms_n);

    void putLMSToBegin0(std::vector <int> &data, std::vector <int> &sorted_suffixes, int lms_n);

    //put LMS suffixes that hasn't been sorted in their blocks(level 1)
    //return number of lms suffixes
    int putLMS1(std::vector <int> &sorted_suffixes,
                 int beg, int size);

    //put sorted LMS suffixes in their blocks(level 1)
    void putSortedLMS1(std::vector <int> &sorted_suffixes,
                      int lms_n, int beg, int size);

    //sort L-suffixes if `lms_suffixes` has been sorted for level > 0
    //data is stored in sorted_suffixes from `beg` to (`beg` + `size`)
    void inducedSort1L(std::vector <int> &sorted_suffixes,
                      int lms_n, int beg, int size, bool sort_lms_substrings);

    //sort S-suffixes if `lms_suffixes` has been sorted for level > 0
    //data is stored in sorted_suffixes from `beg` to (`beg` + `size`)
    void inducedSort1S(std::vector <int> &sorted_suffixes,
                       int lms_n, int beg, int size, bool sort_lms_substring);

    //return the number of distinct values in the new string
    int formNewString1(std::vector <int> &sorted_suffixes, int lms_n, int begin, int size);

    void putLMSToBegin1(std::vector <int> &sorted_suffixes, int lms_n, int beg, int size);

    int EMPTY = INT_MIN;
    friend class SortTests;
};


#endif //SACA_K_SUFFIXSORT_H
