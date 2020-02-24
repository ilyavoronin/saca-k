#include <SuffixSort.h>




int SuffixSort::formNewString0(std::vector<int> &data, std::vector<int> &suffix_array, int lms_n) {
    int n = data.size();
    int block_begin = 0;
    //suffix_array[0] - 1 >= 0 because 0 is not lms
    //if we divide indexes of lms-suffix by 2, we will get different values from 0 to n / 2
    suffix_array[lms_n + (suffix_array[0] - 1) / 2] = block_begin;
    int prev_lms = suffix_array[0];
    int prev_lms_end = suffix_array[0]; // suffix_array[0] is always n - 1
    int new_alph_size = 1;
    for (int i = 1; i < lms_n; i++) {
        int cur_lms = suffix_array[i];
        //we don't know yet where lms-suffix for cur_lms ends
        int k1 = cur_lms + 1;
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

        //know we can compare lms-suffixes cur_lms and prev_lms
        bool is_eq = true;
        if (cur_lms_end - cur_lms != prev_lms_end - prev_lms) {
            is_eq = false;
        }
        else {
            for (int i1 = cur_lms, i2 = prev_lms; i1 <= cur_lms_end; i1++, i2++) {
                if (data[i1] != data[i2]) {
                    is_eq = false;
                    break;
                }
            }
        }
        if (!is_eq) {
            //first element of each block point to the end of the block
            suffix_array[block_begin] = i - 1;
            block_begin = i;
            new_alph_size++;
        }
        suffix_array[lms_n + (suffix_array[i] - 1) / 2] = block_begin;
        prev_lms = cur_lms;
        prev_lms_end = cur_lms_end;
    }
    suffix_array[block_begin] = lms_n - 1;

    //now we should move all values to the end
    int last_free = n - 1;
    for (int i = n - 1; i >= lms_n; i--) {
        if (suffix_array[i] != EMPTY) {
            std::swap(suffix_array[last_free--], suffix_array[i]);
        }
    }

    //assign all S-characters pointer to the end of it's block
    bool cur_type = 0;
    for (int i = n - 2; i > n - 1 - lms_n; i--) {
        if (suffix_array[i] < suffix_array[i + 1]) {
            cur_type = 0;
        }
        if (suffix_array[i] > suffix_array[i + 1]) {
            cur_type = 1;
        }
        if (cur_type == 0) {
            //suffix_array[i] points to the begin of the block
            //and suffix_array[`begin of the block`] points to the end of the block
            suffix_array[i] = suffix_array[suffix_array[i]];
        }
    }
    for (int i = 0; i < lms_n; i++) {
        suffix_array[i] = EMPTY;
    }
    return new_alph_size;
}

int SuffixSort::formNewString1(std::vector<int> &suffix_array, int lms_n, int beg, int size){
    int block_begin = 0;
    int new_alph_size = 0;
    //from where we will store new data
    int new_begin = beg - size / 2;
    //suffix_array[0] - beg - 1 >= 0 because `beg` is not lms
    //and because there is at least one element between lms-indexes we can divide by 2
    //and get distinct values for lms-indexes
    suffix_array[new_begin + (suffix_array[0] - beg - 1) / 2] = block_begin;
    new_alph_size++;
    int prev_lms = suffix_array[0];
    int prev_lms_end = suffix_array[0]; // suffix_array[0] is always (beg + size - 1)
    for (int i = 1; i < lms_n; i++) {
        int cur_lms = suffix_array[i];
        //we don't know yet where lms-substring for `cur_lms` ends
        int k1 = cur_lms + 1;
        int cur_lms_end;
        while (k1 < beg + size) {
            if (suffix_array[k1] < suffix_array[k1 - 1]) {
                break;
            }
            k1++;
        }
        //k1 now points to the L suffix
        int last_s;
        while (k1 < beg + size) {
            if (suffix_array[k1] > suffix_array[k1 - 1]) {
                cur_lms_end = last_s;
                break;
            }
            if (suffix_array[k1] < suffix_array[k1 - 1]) {
                last_s = k1;
            }
            k1++;
        }
        if (k1 == beg + size) {
            cur_lms_end = beg + size - 1;
        }

        //know we can compare lms-suffixes cur_lms and prev_lms
        bool is_eq = true;
        if (cur_lms_end - cur_lms != prev_lms_end - prev_lms) {
            is_eq = false;
        }
        else {
            for (int i1 = cur_lms, i2 = prev_lms; i1 <= cur_lms_end; i1++, i2++) {
                if (suffix_array[i1] != suffix_array[i2]) {
                    is_eq = false;
                    break;
                }
            }
        }
        if (!is_eq) {
            //first element of each block points to the end of the block
            suffix_array[block_begin] = i - 1;
            block_begin = i;
            new_alph_size++;
        }
        suffix_array[new_begin + (suffix_array[i] - beg - 1) / 2] = block_begin;
        prev_lms = cur_lms;
        prev_lms_end = cur_lms_end;
    }
    suffix_array[block_begin] = lms_n - 1;

    //now we should move all values to the end
    int last_free = beg - 1;
    for (int i = beg - 1; i >= new_begin; i--) {
        if (suffix_array[i] != EMPTY) {
            std::swap(suffix_array[last_free--], suffix_array[i]);
        }
    }

    //assign all S-characters pointer to the end of it's block
    bool cur_type = 0;
    for (int i = beg - 2; i > beg - 1 - lms_n; i--) {
        if (suffix_array[i] < suffix_array[i + 1]) {
            cur_type = 0;
        }
        if (suffix_array[i] > suffix_array[i + 1]) {
            cur_type = 1;
        }
        if (cur_type == 0) {
            //suffix_array[i] points to the beg of the block
            //and suffix_array[`beg of the block`] points to the end of the block
            suffix_array[i] = suffix_array[suffix_array[i]];
        }
    }
    for (int i = 0; i < lms_n; i++) {
        suffix_array[i] = EMPTY;
    }
    return new_alph_size;
}