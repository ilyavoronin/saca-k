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

    if (level == 0) {
        int n = data.size();
        int max_symb_number = 0;
        for (int i = 0; i < n; i++) {
            max_symb_number = std::max(data[i], max_symb_number);
        }
        max_symb_number++;
        std::vector <int> block_begin(max_symb_number + 1);
        //put all lms-suffixes in the beginning of `sorted_suffixes`
        //and calculate block_begin
        sorted_suffixes[0] = n - 1;
        int cur_pos = 0;
        bool last_type = 0;
        block_begin[0] = 1;
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

            //checking if suffix is lms
            if (cur_type != 0 && last_type == 0) {
                sorted_suffixes[cur_pos++] = i + 1;
                lms_n++;
            }
            last_type = cur_type;

            block_begin[data[i]]++;
        }
        cur_pos = 0;
        for (int i = 1; i < max_symb_number; i++) {
            int tmp = block_begin[i];
            block_begin[i] = cur_pos;
            cur_pos += tmp;
        }
        block_begin[max_symb_number] = cur_pos;

        inducedSortLevel0(data, block_begin, sorted_suffixes,
                          max_symb_number, lms_n);
    }
    else {
        int lms_n = 0;
        int last_type = 0;
        int cur_pos = 1;
        for (int i = beg + size - 2; i >= beg; i--) {
            int cur_type = 0;
            if (sorted_suffixes[i] < sorted_suffixes[i + 1]) {
                cur_type = 0;
            } else if (sorted_suffixes[i] > sorted_suffixes[i + 1]) {
                cur_type = 1;
            } else {
                cur_type = last_type;
            }
            if (cur_type == 1 && last_type == 0) {
                sorted_suffixes[cur_pos++] = i + 1;
                lms_n++;
            }
            last_type = cur_type;
        }

        inducedSortLevel1(sorted_suffixes, lms_n);
    }
    /*
    std::vector <int> sorted_lms;
    std::vector <int> eq_class(data.size());
    int cur_class = 0;
    int last_lms_index = -1;
    int cc = 0;
    for (int i = 0; i < sorted_suffixes.size(); i++) {
        if (!is_lms[sorted_suffixes[i]]) {
            continue;
        }
        if (last_lms_index == -1) {
            eq_class[sorted_suffixes[i]] = cur_class;
            last_lms_index = sorted_suffixes[i];
            continue;
        }
        int i1 = sorted_suffixes[i];
        int i2 = last_lms_index;
        bool is_eq = true;
        if (data[i1] != data[i2]) {
            is_eq = false;
        }
        else {
            for (int j = 1; j < data.size(); j++) {
                cc++;
                if (data[i1 + j] != data[i2 + j]) {
                    is_eq = false;
                    break;
                }
                if (is_lms[i1 + j] || is_lms[i2 + j]) {
                    if (!is_lms[i1 + j] || !is_lms[i2 + j]) {
                        is_eq = false;
                    }
                    break;
                }
            }
        }

        if (!is_eq) {
            cur_class++;
        }
        eq_class[sorted_suffixes[i]] = cur_class;
        last_lms_index = sorted_suffixes[i];
    }
    std::vector <int> lms_data(lms_suffixes.size());
    for (int i = 0; i < lms_suffixes.size(); i++) {
        lms_data[i] = eq_class[lms_suffixes[i]];
    }
    sorted_suffixes.clear();
    sortSuffixes(lms_data, sorted_suffixes);
    std::vector <int> sorted_lms_suffixes(lms_suffixes.size());
    for (int i = 0; i < sorted_suffixes.size(); i++) {
        sorted_lms_suffixes[i] = lms_suffixes[sorted_suffixes[i]];
    }

    sorted_suffixes.clear();
    inducedSort(data, type, begin_S, begin_L,
                cnt_S, sorted_lms_suffixes,
                sorted_suffixes, max_symb_number);
    */
}


void SuffixSort::inducedSortLevel0(std::vector <int> &data,
                             std::vector <int> &block_begin,
                             std::vector <int> &sorted_suffixes,
                             int max_symb_number,
                             int lms_n) {
    for (int i = lms_n; i < sorted_suffixes.size(); i++) {
        sorted_suffixes[i] == -1;
    }
    std::vector<int> cur_shift(max_symb_number, 0);

    //put lms-suffixes in their places
    for (int i = lms_n - 1; i >= 0; i--) {
        int symb = data[sorted_suffixes[i]];
        int pos = block_begin[symb + 1] - 1 - cur_shift[symb];
        sorted_suffixes[pos] = sorted_suffixes[i];
    }

    //find positions for L-suffixes
    cur_shift.assign(max_symb_number, 0);
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

void SuffixSort::inducedSortLevel1(std::vector<int> &sorted_suffixes, int lms_n) {

}