#ifndef SACA_K_SUFFIXSORT_H
#define SACA_K_SUFFIXSORT_H

#include <vector>
#include <climits>

class SuffixSort {
public:
    void sort(std::vector <int> &data, std::vector <int> &sorted_suffixes);

private:
    //SA-IS algorithm
    void sortSuffixes(std::vector <int> &data, std::vector <int> &sorted_suffixes,
                      int level, int beg, int size);

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
    void inducedSort0(std::vector <int> &data,
                     std::vector <int> &block_begin,
                     std::vector <int> &sorted_suffixes,
                     int max_symb_number,
                     int lms_n);

    //put LMS suffixes that hasn't been sorted in their blocks(level 1)
    //return number of lms suffixes
    int putLMS1(std::vector <int> &sorted_suffixes,
                 int beg, int size);

    //put sorted LMS suffixes in their blocks(level 1)
    void putSortedLMS1(std::vector <int> &sorted_suffixes,
                      int lms_n, int beg, int size);

    //sort suffixes if `lms_suffixes` has been sorted for level > 0
    //data is stored in sorted_suffixes from `beg` to (`beg` + `size`)
    void inducedSort1(std::vector <int> &sorted_suffixes,
                      int lms_n, int beg, int size);

    int EMPTY = INT_MIN;
};


#endif //SACA_K_SUFFIXSORT_H
