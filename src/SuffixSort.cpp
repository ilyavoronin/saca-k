#include "SuffixSort.h"

#include <iostream>

void SuffixSort::sort(std::vector <int> &data, std::vector <int> &sorted_suffixes) {
    for (int i = 0; i < (int)data.size() - 1; i++) {
        data[i]++;
    }
    sortSuffixes(data, sorted_suffixes);
    for (int i = 0; i < (int)data.size() - 1; i++) {
        data[i]--;
    }
}

void SuffixSort::sortSuffixes(std::vector <int> &data, std::vector <int> &sorted_suffixes){
    if (data.size() == 1) {
        sorted_suffixes.push_back(0);
        return;
    }
    int n = data.size();

    //0 -- type S(suf(i) < suf(i + 1)
    //1 -- type L(suf(i) > suf(i + 1)
    std::vector <bool> type(n);

    //i is a LMS(leftmost S) if type[i] = S, type[i - 1] = L
    std::vector <int> lms_suffixes;
    std::vector <bool> is_lms(data.size(), false);

    int max_symb_number = 0;
    for (int i = 0; i < n; i++) {
        max_symb_number = std::max(data[i], max_symb_number);
    }
    max_symb_number++;

    //number of suffixes that starts with symbol `i` and has type S/L
    std::vector <int> cnt_S(max_symb_number, 0);
    std::vector <int> cnt_L(max_symb_number, 0);

    std::vector <int> begin_S(max_symb_number, 0);
    std::vector <int> begin_L(max_symb_number, 0);

    type[n - 1] = 0;
    cnt_S[0] = 1;
    for (int i = n - 2; i >= 0; i--) {
        if (data[i] < data[i + 1]) {
            type[i] = 0;
        }
        if (data[i] > data[i + 1]) {
            type[i] = 1;
        }
        if (data[i] == data[i + 1]) {
            type[i] = type[i + 1];
        }

        if (type[i] == 0) {
            cnt_S[data[i]]++;
        }
        else {
            cnt_L[data[i]]++;
        }
    }

    //position at which the S/L block of corresponding symbol starts
    int cur_pos = 0;
    for (int i = 0; i < max_symb_number; i++) {
        begin_L[i] = cur_pos;
        cur_pos += cnt_L[i];
        begin_S[i] = cur_pos;
        cur_pos += cnt_S[i];
    }

    for (int i = 0; i < n - 1; i++) {
        if (type[i] == 1 && type[i + 1] == 0) {
            lms_suffixes.push_back(i + 1);
            is_lms[i + 1] = true;
        }
    }

    inducedSort(data, type, begin_S,
                begin_L, cnt_S, lms_suffixes,
                sorted_suffixes, max_symb_number);
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
}


void SuffixSort::inducedSort(std::vector <int> &data,
                             std::vector <bool> &type,
                             std::vector <int> &begin_S,
                             std::vector <int> &begin_L,
                             std::vector <int> &cnt_S,
                             std::vector<int> &lms_suffixes,
                             std::vector<int> &sorted_suffixes,
                             int max_symb_number) {
    sorted_suffixes.assign(data.size(), -1);
    //how many symbols have been added to the S block of corresponding symbol
    std::vector<int> cur_shift_S(max_symb_number, 0);

    for (auto suffix : lms_suffixes) {
        int symb = data[suffix];
        sorted_suffixes[begin_S[symb] + cur_shift_S[symb]] = suffix;
        cur_shift_S[symb]++;
    }

    //find positions for L-suffixes
    std::vector<int> cur_shift_L(max_symb_number, 0);
    for (int i = 0; i < sorted_suffixes.size(); i++) {
        if (sorted_suffixes[i] != -1 && sorted_suffixes[i] != 0 &&
            type[sorted_suffixes[i] - 1] == 1) {
            int suffix = sorted_suffixes[i] - 1;
            int symb = data[suffix];
            sorted_suffixes[begin_L[symb] + cur_shift_L[symb]] = suffix;
            cur_shift_L[symb]++;
        }
    }

    //find positions for S-suffixes
    cur_shift_S.assign(max_symb_number, 0);
    for (int i = (int) sorted_suffixes.size() - 1; i >= 0; i--) {
        if (sorted_suffixes[i] != -1 && sorted_suffixes[i] != 0 &&
            type[sorted_suffixes[i] - 1] == 0) {
            int suffix = sorted_suffixes[i] - 1;
            int symb = data[suffix];
            int block_end = begin_S[symb] + cnt_S[symb] - 1;
            sorted_suffixes[block_end - cur_shift_S[symb]] = suffix;
            cur_shift_S[symb]++;
        }
    }
}