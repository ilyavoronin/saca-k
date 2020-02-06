#include "SuffixSort.h"


void SuffixSort::sort(std::vector <int> &data, std::vector <int> &sorted_suffixes) {
    for (int i = 0; i < (int)data.size() - 1; i++) {
        data[i]++;
    }
    data.push_back(0);
    sorted_suffixes.resize(data.size());
    sortSuffixes0(data, sorted_suffixes);
    for (int i = 0; i < (int)data.size() - 1; i++) {
        data[i]--;
    }
}

void SuffixSort::sortSuffixes0(std::vector <int> &data, std::vector <int> &sorted_suffixes) {
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
    sortSuffixes1(sorted_suffixes, data.size() - lms_n, data.size());

    //put all lms-suffixes in the beginning of the `sorted_suffixes` in the right order
    //suffix array is stored from 0 to lms_n

    //at first store all lms-suffixes in the `sorted_suffixes` from lms_n to 2 * lms_n
    int n = data.size();
    //current position for lms-suffix
    int last_pos = lms_n * 2 - 1;
    int last_type = 0, cur_type;
    for (int i = (int) data.size() - 2; i >= 0; i--) {
        if (data[i] == data[i + 1]) {
            cur_type = last_type;
        } else {
            if (data[i] < data[i + 1]) {
                cur_type = 0;
            } else {
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

void SuffixSort::sortSuffixes1(std::vector <int> &sorted_suffixes, int beg, int size){
    if (size == 1) {
        sorted_suffixes[0] = 0;
        return;
    }
    for (int i = 0; i < size; i++) {
        sorted_suffixes[i] = EMPTY;
    }
    int lms_n = putLMS1(sorted_suffixes, beg, size);
    inducedSort1(sorted_suffixes, lms_n, beg, size);

    //TODO sort lms substrings

    putSortedLMS1(sorted_suffixes, lms_n, beg, size);
    inducedSort1(sorted_suffixes, lms_n, beg, size);
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

    //assign all S-characters pointer to the end of it's block
    bool cur_type = 0;
    for (int i = n - 2; i > n - 1 - lms_n; i--) {
        if (data[i] < data[i + 1]) {
            cur_type = 0;
        }
        if (data[i] > data[i + 1]) {
            cur_type = 1;
        }
        if (cur_type == 0) {
            //sorted_suffixes[i] points to the begin of the block
            //and sorted_suffixes[`begin of the block`] points to the end of the block
            sorted_suffixes[i] = sorted_suffixes[sorted_suffixes[i]];
        }
    }
}

int SuffixSort::putLMS1(std::vector<int> &sorted_suffixes, int beg, int size) {
    bool last_type = 0, cur_type;
    int lms_n = 0;
    for (int i = beg + size - 2; i >= beg; i--) {
        if (sorted_suffixes[i] == sorted_suffixes[i + 1]) {
            cur_type = last_type;
        }
        else {
            if (sorted_suffixes[i] < sorted_suffixes[i + 1]) {
                cur_type = 0;
            }
            else {
                cur_type = 1;
            }
        }
        if (cur_type == 1 && last_type == 0) {
            //then this is lms-suffix and we need to find position for it
            //sorted_suffixes[i] points to the end of it's block
            lms_n++;
            int block_end = sorted_suffixes[i];
            if (block_end == 0) {
                sorted_suffixes[block_end] = sorted_suffixes[i];
                continue;
            }
            if (sorted_suffixes[block_end] == EMPTY) {
                if (sorted_suffixes[block_end - 1] == EMPTY) {
                    sorted_suffixes[block_end - 1] = sorted_suffixes[i];
                    //-sorted_suffixes[j] shows how many elements(+1) have been already stored
                    sorted_suffixes[block_end] = -2;
                }
                else {
                    //sorted_suffixes[i] is the only element in it's block
                    sorted_suffixes[block_end] = sorted_suffixes[i];
                }
            }
            else if(sorted_suffixes[block_end] < 0) {
                //TODO make sure that k >= 0
                //points to the free space in the block
                int k = block_end + sorted_suffixes[block_end]; //in fact the value subtracts
                if (sorted_suffixes[k] == EMPTY) {
                    sorted_suffixes[k] = sorted_suffixes[i];
                    //increase the element counter
                    sorted_suffixes[block_end]--;
                } else {
                    //block is full, we need to shift the elements to the right
                    int prev_elem = EMPTY;
                    for (int u = k + 1; u <= block_end; u++) {
                        std::swap(prev_elem, sorted_suffixes[u]);
                    }
                    sorted_suffixes[k + 1] = sorted_suffixes[i];
                }
            } else { //sorted_suffixes[block_end] >= 0
                //then sorted_suffixes[block_end] is value from another block and
                //we should shift that block to the right
                int k = block_end;
                //while sorted_suffixes[k] is not counter
                int last_value = EMPTY;
                while (sorted_suffixes[k] >= 0) {
                    std::swap(sorted_suffixes[k], last_value);
                    k++;
                }

                //then perform the same action as in the first case
                if (sorted_suffixes[block_end - 1] == EMPTY) {
                    sorted_suffixes[block_end - 1] = sorted_suffixes[i];
                    //-sorted_suffixes[j] shows how many elements(+1) have been already stored
                    sorted_suffixes[block_end] = -2;
                }
                else {
                    //sorted_suffixes[i] is the only element in it's block
                    sorted_suffixes[block_end] = sorted_suffixes[i];
                }
            }
        }
        last_type = cur_type;
    }
    //now we need to shift right all the values in the block,
    //if the block still has counter
    int i = size - 1;
    while (i >= 0) {
        if (sorted_suffixes[i] < 0 && sorted_suffixes[i] != EMPTY) {
            //then sorted_suffixes[i] is a counter
            int j = i;
            i = i + sorted_suffixes[i];
            while (j > i + sorted_suffixes[i]) {
                sorted_suffixes[j] = sorted_suffixes[j - 1];
                sorted_suffixes[j - 1] = EMPTY;
                j--;
            }
        }
        i--;
    }
    return lms_n;
}

void SuffixSort::putSortedLMS1(std::vector<int> &sorted_suffixes,
                               int lms_n, int beg, int size) {

}

void SuffixSort::inducedSort1L(std::vector<int> &sorted_suffixes, int lms_n,
                                   int beg, int size) {

}

void SuffixSort::inducedSort1S(std::vector<int> &sorted_suffixes, int lms_n,
                               int beg, int size) {

}