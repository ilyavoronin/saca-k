#include "SuffixSort.h"

#include <iostream>

void SuffixSort::sort(std::vector <int> &data, std::vector <int> &sorted_suffixes) {
    for (int i = 0; i < (int)data.size() - 1; i++) {
        data[i]++;
    }
    sorted_suffixes.resize(data.size());
    sortSuffixes(data, sorted_suffixes, 0, 0, data.size());
    for (int i = 0; i < (int)data.size() - 1; i++) {
        data[i]--;
    }
}

//0 -- S type, 1 -- L-type
//for levels > 0 data is stored in `sorted_suffixes` from `beg` to (`beg` + `size`)
void SuffixSort::sortSuffixes(std::vector <int> &data, std::vector <int> &sorted_suffixes,
                              int level, int beg, int size){
    if (data.size() == 1) {
        sorted_suffixes.push_back(0);
        return;
    }
    std::vector <int> block_begin;
    if (level == 0) {
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
        for (int i = 1; i < max_symb_number; i++) {
            int tmp = block_begin[i];
            block_begin[i] = cur_pos;
            cur_pos += tmp;
        }
        block_begin[max_symb_number] = cur_pos;

        int lms_n = putLMS0(data, block_begin, sorted_suffixes, max_symb_number);
        inducedSort0(data, block_begin, sorted_suffixes,
                          max_symb_number, lms_n);

        //TODO sort lms strings

        putSortedLMS0(data, block_begin, sorted_suffixes, max_symb_number, lms_n);
        inducedSort0(data, block_begin, sorted_suffixes, max_symb_number, lms_n);
    }
    else {

    }
}

int SuffixSort::putLMS0(std::vector<int> &data, std::vector<int> &block_begin,
                         std::vector<int> &sorted_suffixes,
                         int max_symb_number) {
    int n = data.size();
    std::vector <int> cur_shift(max_symb_number, 0);
    sorted_suffixes[0] = n - 1;
    int cur_pos = 0;
    bool last_type = 0;
    int lms_n = 0;
    for (int i = n - 2; i >= 0; i--) {
        bool cur_type = 0;
        if (data[i] < data[i + 1]) {
            cur_type = 0;
        }
        else if (data[i] > data[i + 1]) {
            cur_type = 1;
        } else {
            cur_type = last_type;
        }

        //checking if (i + 1) is lms suffix
        if (cur_type != 0 && last_type == 0) {
            int symb = data[i + 1];
            int pos = block_begin[symb] + cur_shift[symb];
            sorted_suffixes[pos] = i + 1;
            cur_shift[symb]++;
            lms_n++;
        }
        last_type = cur_type;
    }
    return lms_n;
}


void SuffixSort::putSortedLMS0(std::vector<int> &data, std::vector<int> &block_begin,
                               std::vector<int> &sorted_suffixes,
                               int max_symb_number, int lms_n) {
    for (int i = lms_n; i < sorted_suffixes.size(); i++) {
        sorted_suffixes[i] = -1;
    }
    std::vector<int> cur_shift(max_symb_number, 0);

    //put lms-suffixes in their places
    for (int i = lms_n - 1; i >= 0; i--) {
        int symb = data[sorted_suffixes[i]];
        int pos = block_begin[symb + 1] - 1 - cur_shift[symb];
        sorted_suffixes[pos] = sorted_suffixes[i];
        sorted_suffixes[i] = -1;
    }
}

//put L and S suffixes in there places
void SuffixSort::inducedSort0(std::vector <int> &data,
                             std::vector <int> &block_begin,
                             std::vector <int> &sorted_suffixes,
                             int max_symb_number,
                             int lms_n) {

    std::vector <int> cur_shift(max_symb_number, 0);
    //find positions for L-suffixes
    for (int i = 0; i < sorted_suffixes.size(); i++) {
        int j = sorted_suffixes[i];
        int symb = data[j - 1];
        if (sorted_suffixes[i] > 0 &&
            data[j - 1] >= data[j]) {
            sorted_suffixes[block_begin[symb] + cur_shift[symb]] = j - 1;
            cur_shift[symb]++;
        }
    }

    //find positions for S-suffixes
    cur_shift.assign(max_symb_number, 0);
    for (int i = (int) sorted_suffixes.size() - 1; i >= 0; i--) {
        int j = sorted_suffixes[i];
        int symb = data[j - 1];
        int pos = block_begin[symb + 1] - 1 - cur_shift[symb];
        bool is_s_type = data[j - 1] < data[j] ||
                         (data[j - 1] == data[j] && pos < i);
        if (sorted_suffixes[i] > 0 && is_s_type) {
            sorted_suffixes[pos] = j - 1;
            cur_shift[symb]++;
        }
    }
}

void SuffixSort::inducedSortLevel1(std::vector<int> &sorted_suffixes, int lms_n,
                                   int beg, int size) {
    for (int i = lms_n; i < size; i++) {
        sorted_suffixes[i] = EMPTY;
    }

    //put lms-suffixes in there places
    for (int i = lms_n - 1; i >= 0; i--) {

    }
}