#include "SuffixSort.h"


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
        sorted_suffixes.push_back(0); //something wrong, i can feel it
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
                          max_symb_number, true);

        formNewString0(data, sorted_suffixes, lms_n);
        sortSuffixes(data, sorted_suffixes, 1, data.size() - lms_n, data.size());

        //put all lms-suffixes in the beginning of the `sorted_suffixes` in the right order
        //suffix array is stored from 0 to lms_n

        //at first store all lms-suffixes in the `sorted_suffixes` from lms_n to 2 * lms_n
        int n = data.size();
        //current position for lms-suffix
        int last_pos = lms_n * 2 - 1;
        int last_type = 0, cur_type;
        for (int i = (int)data.size() - 2; i >= 0; i--) {
            if (data[i] == data[i + 1]) {
                cur_type = last_type;
            }
            else {
                if (data[i] < data[i + 1]) {
                    cur_type = 0;
                }
                else {
                    cur_type = 1;
                }
            }
            if (cur_type == 1 && last_type == 0) {
                sorted_suffixes[last_pos--] = i + 1;
            }
            last_type = cur_type;
        }

        //put all lms-suffixes in the right places in `sorted_suffixes`
        for (int i = 0; i < lms_n; i++) {
            //let j = sorted_suffixes[i]
            //sorted_suffixes[lms_n + j] -- index of the lms-suffix number j
            sorted_suffixes[i] = sorted_suffixes[lms_n + sorted_suffixes[i]];
        }

        putSortedLMS0(data, block_begin, sorted_suffixes, max_symb_number, lms_n);
        inducedSort0(data, block_begin, sorted_suffixes, max_symb_number, false);
    }
    else {
        int lms_n = putLMS1(sorted_suffixes, beg, size);
        inducedSort1(sorted_suffixes, lms_n, beg, size);

        //TODO sort lms substrings

        putSortedLMS1(sorted_suffixes, lms_n, beg, size);
        inducedSort1(sorted_suffixes, lms_n, beg, size);
    }
}

int SuffixSort::putLMS0(std::vector<int> &data, std::vector<int> &block_begin,
                         std::vector<int> &sorted_suffixes,
                         int max_symb_number) {
    int n = data.size();
    std::vector <int> cur_shift(max_symb_number, 0);
    sorted_suffixes[0] = n - 1;
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

            //put lms suffix in the end of corresponding block
            int pos;
            if (symb == 0) {
                pos = 0;
            }
            else {
                pos = block_begin[symb + 1] - cur_shift[symb] - 1;
            }
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
        int pos = block_begin[symb + 1] -cur_shift[symb] - 1;
        sorted_suffixes[pos] = sorted_suffixes[i];
        cur_shift[symb]++;
        sorted_suffixes[i] = -1;
    }
}

//put L and S suffixes in there places
void SuffixSort::inducedSort0(std::vector <int> &data,
                             std::vector <int> &block_begin,
                             std::vector <int> &sorted_suffixes,
                             int max_symb_number,
                             bool sort_lms_substrings) {

    std::vector <int> cur_shift(max_symb_number, 0);
    //find positions for L-suffixes
    for (int i = 0; i < sorted_suffixes.size(); i++) {
        int j = sorted_suffixes[i];
        if (j <= 0) {
            continue;
        }
        int symb = data[j - 1];
        if (data[j - 1] >= data[j]) {
            sorted_suffixes[block_begin[symb] + cur_shift[symb]] = j - 1;
            cur_shift[symb]++;
            if (sort_lms_substrings) {
                //we don't need this value anymore
                sorted_suffixes[i] = -1;
            }
        }
    }

    //find positions for S-suffixes
    cur_shift.assign(max_symb_number, 0);
    for (int i = (int) sorted_suffixes.size() - 1; i >= 0; i--) {
        int j = sorted_suffixes[i];
        if (j <= 0) {
            continue;
        }
        int symb = data[j - 1];
        int pos = block_begin[symb + 1] - 1 - cur_shift[symb];
        bool is_s_type = data[j - 1] < data[j] ||
                         (data[j - 1] == data[j] && pos < i);
        if (sorted_suffixes[i] > 0 && is_s_type) {
            sorted_suffixes[pos] = j - 1;
            cur_shift[symb]++;
            if (sort_lms_substrings) {
                //we don't need this value anymore
                sorted_suffixes[i] = -1;
            }
        }
    }

    if (sort_lms_substrings) {
        //now indexes that corresponds to the lms-suffixes are stored
        //in `sorted_suffixes` in the right order,
        //and other values is set to -1
        //we want to move all the lms-suffixes to the front
        int j = 0;
        for (int i = 0; i < (int)sorted_suffixes.size(); i++) {
            if (sorted_suffixes[i] > 0) {
                sorted_suffixes[j] = sorted_suffixes[i];
            }
        }
    }
}

void SuffixSort::formNewString0(std::vector<int> &data, std::vector<int> &sorted_suffixes, int lms_n) {
    int n = data.size();
    int block_begin = 0;
    //if we divide indexes of lms-suffix by 2, we will get different values from 0 to n / 2
    sorted_suffixes[lms_n + sorted_suffixes[0] / 2] = block_begin;
    int prev_lms_end = sorted_suffixes[0]; // sorted_suffixes[0] is always n - 1
    for (int i = 1; i < lms_n; i++) {
        int i1 = sorted_suffixes[i];
        int j1 = sorted_suffixes[i - 1];
        //we don't know yet where lms-suffix for i1 ends
        int k1 = i1 + 1;
        int cur_lms_end;
        while (k1 < n) {
            if (data[k1] < data[k1 - 1]) {
                break;
            }
            k1++;
        }
        //k1 now points to the L suffix
        int last_s;
        while (k1 < n) {
            if (data[k1] > data[k1 - 1]) {
                cur_lms_end = last_s;
                break;
            }
            if (data[k1] < data[k1 - 1]) {
                last_s = k1;
            }
            k1++;
        }
        if (k1 == n) {
            cur_lms_end = n - 1;
        }

        //know we can compare lms-suffixes i1 and j1
        bool is_eq = true;
        if (cur_lms_end - i1 != prev_lms_end - j1) {
            is_eq = false;
        }
        else {
            for (; i1 <= cur_lms_end && j1 <= prev_lms_end; i1++, j1++) {
                if (data[i1] != data[j1]) {
                    is_eq = false;
                    break;
                }
            }
        }
        if (!is_eq) {
            //first element of each block point to the end of the block
            sorted_suffixes[block_begin] = i - 1;
            block_begin = i;
        }
        sorted_suffixes[lms_n + sorted_suffixes[i] / 2] = block_begin;
    }
    sorted_suffixes[block_begin] = lms_n - 1;

    //now we should move all values to the end to the end
    //TODO check if empty array values == -1
    int last_free = n - 1;
    for (int i = n - 1; i >= lms_n; i--) {
        if (sorted_suffixes[i] != -1) {
            sorted_suffixes[last_free--] = sorted_suffixes[i];
        }
    }

    //assign all L-characters pointer to the end of it's block
    bool cur_type = 0;
    for (int i = n - 2; i > n - 1 - lms_n; i--) {
        if (data[i] < data[i + 1]) {
            cur_type = 0;
        }
        if (data[i] > data[i + 1]) {
            cur_type = 1;
        }
        if (cur_type == 1) {
            //sorted_suffixes[i] points to the begin of the block
            //and sorted_suffixes[`begin of the block`] points to the end of the block
            sorted_suffixes[i] = sorted_suffixes[sorted_suffixes[i]];
        }
    }
}

int SuffixSort::putLMS1(std::vector<int> &sorted_suffixes, int beg, int size) {
}

void SuffixSort::putSortedLMS1(std::vector<int> &sorted_suffixes,
                               int lms_n, int beg, int size) {

}

void SuffixSort::inducedSort1(std::vector<int> &sorted_suffixes, int lms_n,
                                   int beg, int size) {
    for (int i = lms_n; i < size; i++) {
        sorted_suffixes[i] = EMPTY;
    }

    //put lms-suffixes in there places
    for (int i = lms_n - 1; i >= 0; i--) {

    }
}