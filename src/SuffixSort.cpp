#include "SuffixSort.h"




void SuffixSort::build_suffix_array(std::vector <int> &data, std::vector <int> &suffix_array) {
    if (data.size() == 0) {
        suffix_array.clear();
        return;
    }
    for (int i = 0; i < (int)data.size(); i++) {
        data[i]++;
    }
    data.push_back(0);
    suffix_array.resize(data.size(), EMPTY);
    sortSuffixes0(data, suffix_array);
    data.pop_back();
    for (int i = 0; i < (int)suffix_array.size() - 1; i++) {
        suffix_array[i] = suffix_array[i + 1];
    }
    suffix_array.pop_back();
    for (int i = 0; i < (int)data.size(); i++) {
        data[i]--;
    }
}

void SuffixSort::sortSuffixes0(std::vector <int> &data, std::vector <int> &suffix_array) {
    std::vector<int> block_begin;
    int max_symb_number = 0;
    for (int i = 0; i < data.size(); i++) {
        max_symb_number = std::max(data[i], max_symb_number);
    }
    max_symb_number++;

    //calculate block start
    block_begin.resize(max_symb_number + 1);
    for (int i = 0; i < data.size(); i++) {
        block_begin[data[i]]++;
    }

    int cur_pos = 0;
    for (int i = 0; i < max_symb_number; i++) {
        int tmp = block_begin[i];
        block_begin[i] = cur_pos;
        cur_pos += tmp;
    }
    block_begin[max_symb_number] = cur_pos;

    int lms_n = putLMS0(data, block_begin, suffix_array, max_symb_number);
    inducedSort0(data, block_begin, suffix_array,
                 max_symb_number, true);

    int n_distinct = formNewString0(data, suffix_array, lms_n);
    if (n_distinct == lms_n) {
        //then all blocks contain only one index
        for (int i = data.size() - lms_n; i < data.size(); i++) {
            //suffix_array[i] also points to begin/end(in this case end == begin) of the block
            suffix_array[suffix_array[i]] = i;
        }
    }
    else {
        sortSuffixes1(suffix_array, data.size() - lms_n, lms_n);
    }

    //put all lms-suffixes in the beginning of the `suffix_array` in the right order
    //suffix array is stored from 0 to lms_n
    putLMSToBegin0(data, suffix_array, lms_n);

    putSortedLMS0(data, block_begin, suffix_array, max_symb_number, lms_n);
    inducedSort0(data, block_begin, suffix_array, max_symb_number, false);
}

void SuffixSort::sortSuffixes1(std::vector <int> &suffix_array, int beg, int size){
    int lms_n = putLMS1(suffix_array, beg, size);
    inducedSort1L(suffix_array, beg, size, true);
    inducedSort1S(suffix_array, beg, size, true);

    int n_distinct = formNewString1(suffix_array, lms_n, beg, size);
    if (n_distinct == lms_n) {
        //then all blocks contain only one index
        for (int i = beg - lms_n; i < beg; i++) {
            //suffix_array[i] also points to begin/end(in this case end == begin) of the block
            suffix_array[suffix_array[i]] = i;
        }
    }
    else {
        sortSuffixes1(suffix_array, beg - lms_n, lms_n);
    }

    putLMSToBegin1(suffix_array, lms_n, beg, size);

    putSortedLMS1(suffix_array, lms_n);
    inducedSort1L(suffix_array, beg, size, false);
    inducedSort1S(suffix_array, beg, size, false);
}
