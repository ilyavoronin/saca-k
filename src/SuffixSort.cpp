#include "SuffixSort.h"


void SuffixSort::build_suffix_array(std::vector <int> &data, std::vector <int> &suffix_array) {
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
        for (int i = data.size() - lms_n; i < lms_n; i++) {
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

int SuffixSort::putLMS0(std::vector<int> &data, std::vector<int> &block_begin,
                         std::vector<int> &suffix_array,
                         int max_symb_number) {
    int n = data.size();
    std::vector <int> cur_shift(max_symb_number, 0);
    suffix_array[0] = n - 1;
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
            suffix_array[pos] = i + 1;
            cur_shift[symb]++;
            lms_n++;
        }
        last_type = cur_type;
    }
    return lms_n;
}


void SuffixSort::putSortedLMS0(std::vector<int> &data, std::vector<int> &block_begin,
                               std::vector<int> &suffix_array,
                               int max_symb_number, int lms_n) {
    std::vector<int> cur_shift(max_symb_number, 0);

    //put lms-suffixes in their places
    for (int i = lms_n - 1; i >= 0; i--) {
        int symb = data[suffix_array[i]];
        int pos = block_begin[symb + 1] - cur_shift[symb] - 1;
        std::swap(suffix_array[pos], suffix_array[i]);
        cur_shift[symb]++;
    }
}

//put L and S suffixes in there places
void SuffixSort::inducedSort0(std::vector <int> &data,
                             std::vector <int> &block_begin,
                             std::vector <int> &suffix_array,
                             int max_symb_number,
                             bool sort_lms_substrings) {

    std::vector <int> cur_shift(max_symb_number, 0);
    //find positions for L-suffixes
    for (int i = 0; i < suffix_array.size(); i++) {
        int j = suffix_array[i];
        if (j <= 0) {
            continue;
        }
        int symb = data[j - 1];
        if (data[j - 1] >= data[j]) {
            suffix_array[block_begin[symb] + cur_shift[symb]] = j - 1;
            cur_shift[symb]++;
            if (sort_lms_substrings) {
                //we don't need this value anymore
                suffix_array[i] = EMPTY;
            }
        }
    }
    //we've deleted the last lms-index, but we need it further
    suffix_array[0] = data.size() - 1;

    //find positions for S-suffixes
    cur_shift.assign(max_symb_number, 0);
    for (int i = (int) suffix_array.size() - 1; i >= 0; i--) {
        int j = suffix_array[i];
        if (j <= 0) {
            continue;
        }
        int symb = data[j - 1];
        int pos = block_begin[symb + 1] - 1 - cur_shift[symb];
        bool is_s_type = data[j - 1] < data[j] ||
                         (data[j - 1] == data[j] && pos < i);
        if (suffix_array[i] > 0 && is_s_type) {
            suffix_array[pos] = j - 1;
            cur_shift[symb]++;
            if (sort_lms_substrings) {
                //we don't need this value anymore
                suffix_array[i] = EMPTY;
                if (j - 1 == 0) {
                    //0 is not lms
                    suffix_array[pos] = EMPTY;
                }
            }
        }
    }

    if (sort_lms_substrings) {
        //now indexes that corresponds to the lms-suffixes are stored
        //in `suffix_array` in the right order,
        //and other values is set to EMPTY
        //we want to move all the lms-suffixes to the front
        int j = 0;
        for (int i = 0; i < (int)suffix_array.size(); i++) {
            if (suffix_array[i] > 0) {
                std::swap(suffix_array[j++], suffix_array[i]);
            }
        }
    }
}

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
            for (; cur_lms <= cur_lms_end && prev_lms <= prev_lms_end; cur_lms++, prev_lms++) {
                if (data[cur_lms] != data[prev_lms]) {
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

void SuffixSort::putLMSToBegin0(std::vector <int> &data, std::vector <int> &suffix_array, int lms_n) {
    //at first store all lms-suffixes in the `suffix_array` from lms_n to 2 * lms_n
    int n = data.size();
    //current position for lms-suffix
    int last_pos = n - 1;
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
            suffix_array[last_pos--] = i + 1;
        }
        last_type = cur_type;
    }

    //put all lms-suffixes in the right places in `suffix_array`
    for (int i = 0; i < lms_n; i++) {
        //let j = suffix_array[i]
        //suffix_array[lms_n + j] -- index of the lms-suffix number j
        int j = suffix_array[i];
        suffix_array[i] = suffix_array[j];
        suffix_array[j] = EMPTY;
    }
}

int SuffixSort::putLMS1(std::vector<int> &suffix_array, int beg, int size) {
    bool last_type = 0, cur_type;
    int lms_n = 0;
    for (int i = beg + size - 2; i >= beg; i--) {
        if (suffix_array[i] == suffix_array[i + 1]) {
            cur_type = last_type;
        }
        else {
            if (suffix_array[i] < suffix_array[i + 1]) {
                cur_type = 0;
            }
            else {
                cur_type = 1;
            }
        }
        if (cur_type == 1 && last_type == 0) {
            //then (i + 1) is lms-suffix and we need to find position for it
            //suffix_array[i] points to the end of it's block
            lms_n++;
            int block_end = suffix_array[i + 1];
            if (block_end == 0) {
                suffix_array[block_end] = i + 1;
                continue;
            }
            if (suffix_array[block_end] == EMPTY) {
                if (suffix_array[block_end - 1] == EMPTY) {
                    suffix_array[block_end - 1] = i + 1;
                    //-suffix_array[j] shows how many elements(+1) have been already stored
                    suffix_array[block_end] = -2;
                }
                else {
                    //suffix_array[i] is the only element in it's block
                    suffix_array[block_end] = i + 1;
                }
            }
            else if(suffix_array[block_end] != EMPTY && suffix_array[block_end] < 0) {
                //points to the free space in the block
                int k = block_end + suffix_array[block_end]; //in fact the value subtracts
                if (suffix_array[k] == EMPTY) {
                    suffix_array[k] = i + 1;
                    //increase the element counter
                    suffix_array[block_end]--;
                } else {
                    //block is full, we need to shift the elements to the right
                    int prev_elem = EMPTY;
                    for (int u = k + 1; u <= block_end; u++) {
                        std::swap(prev_elem, suffix_array[u]);
                    }
                    suffix_array[k + 1] = i + 1;
                }
            } else { //suffix_array[block_end] >= 0
                //then suffix_array[block_end] is value from another block and
                //we should shift that block to the right
                int k = block_end;
                //while suffix_array[k] is not counter
                int last_value = EMPTY;
                while (suffix_array[k] >= 0 || suffix_array[k] == EMPTY) {
                    std::swap(suffix_array[k], last_value);
                    k++;
                }

                //then perform the same actions as in the first case
                if (suffix_array[block_end - 1] == EMPTY) {
                    suffix_array[block_end - 1] = i + 1;
                    //-suffix_array[j] shows how many elements(+1) have been already stored
                    suffix_array[block_end] = -2;
                }
                else {
                    //suffix_array[i] is the only element in it's block
                    suffix_array[block_end] = i + 1;
                }
            }
        }
        last_type = cur_type;
    }
    //now we need to shift right all the values in the block,
    //if the block still has counter
    int i = size - 1;
    while (i >= 0) {
        if (suffix_array[i] < 0 && suffix_array[i] != EMPTY) {
            //then suffix_array[i] is a counter
            int j = i;
            i = i + suffix_array[i] + 1;
            while (j > i) {
                suffix_array[j] = suffix_array[j - 1];
                j--;
            }
            suffix_array[j] = EMPTY;
        }
        i--;
    }
    return lms_n;
}

void SuffixSort::putSortedLMS1(std::vector<int> &suffix_array, int lms_n) {
    //last position in the current block(block where we stored previous lms-index)
    int last_pos = -1;
    int last_lms_symb = -1;
    for (int i = lms_n - 1; i >= 0; i--) {
        int j = suffix_array[i];
        if (suffix_array[j] != last_lms_symb) {
            //then we put all lms-suffixes in the last block and now we switch to the new block
            last_pos = suffix_array[j]; //because suffix_array[j] points to the end of the block
        }
        last_lms_symb = suffix_array[j];
        std::swap(suffix_array[last_pos--], suffix_array[i]);
    }
}

void SuffixSort::inducedSort1L(std::vector<int> &suffix_array,
                                   int beg, int size, bool sort_lms_substrings) {
    for (int i = 0; i < size; i++) {
        int j = suffix_array[i];
        if (j < 0 || j == beg) {
            continue;
        }
        int new_i = i;
        if (suffix_array[j - 1] >= suffix_array[j]) {
            //then j - 1 is l-suffix and we want to store it in suffix_array
            int block_begin = suffix_array[j - 1];
            if (suffix_array[block_begin] == EMPTY) {
                if (suffix_array[block_begin + 1] == EMPTY) {
                    suffix_array[block_begin + 1] = j - 1;
                    //-suffix_array[block_begin] shows how many elements(+1) have been stored
                    suffix_array[block_begin] = -2;
                }
                else {
                    //j - 1 is the only element in the block
                    suffix_array[block_begin] = j - 1;
                }
            }
            else if (suffix_array[block_begin] != EMPTY && suffix_array[block_begin] < 0) {
                //points to free position in the block
                int k = block_begin - suffix_array[block_begin]; //in fact the value is added
                if (suffix_array[k] == EMPTY) {
                    suffix_array[k] = j - 1;
                    //increase the element counter
                    suffix_array[block_begin]--;
                }
                else {
                    //the block is full and we need to shift all elements to the left
                    int prev_elem = EMPTY;
                    for (int u = k - 1; u >= block_begin; u--) {
                        std::swap(prev_elem, suffix_array[u]);
                    }
                    if (suffix_array[j - 1] == suffix_array[j]) {
                        //in this case index `suffix_array[i]` is also l-type
                        //and is stored in the same block and we've shifted all the block to the left
                        //so the we don't need to increment `i` in the for cycle
                        new_i = i - 1;
                    }
                    suffix_array[k - 1] = j - 1;
                }
            } else {  //suffix_array[block_begin] >= 0
                //then suffix_array[block_begin] is value from another block and
                //we should shift that block to the left

                int k = block_begin;
                //while suffix_array[k] is not counter
                int last_value = EMPTY;
                while (suffix_array[k] >= 0 || suffix_array[k] == EMPTY) {
                    std::swap(suffix_array[k], last_value);
                    k--;
                }
                suffix_array[k] = last_value;
                if (k < i) {
                    //in this case `i` is lms-type index and is stored to the left of the block begin
                    //and if k < i then we shifted i-th element to the left in the while cycle
                    new_i = i - 1;
                }

                //then perform the same action as in the first case
                if (suffix_array[block_begin + 1] == EMPTY) {
                    suffix_array[block_begin + 1] = j - 1;
                    //-suffix_array[block_begin] shows how many elements(+1) have been stored
                    suffix_array[block_begin] = -2;
                }
                else {
                    //j - 1 is the only element in the block
                    suffix_array[block_begin] = j - 1;
                }
            }

            //further we take into account, if we've shifted the i-th element
            i = new_i;

            //if suffix_array[i](= j) is lms(<=> not l-type) we need to make it EMPTY
            //so when we will sort S-suffixes, the whole S-block will be EMPTY
            j = suffix_array[i]; //just a reminder
            //isL1 and isL2 is conditions that guarantee that j is l-type(i.e. (j is l-type) = isL1 | isL2)
            //if j == beg + size - 1 then j is the last symbol index, and last symbol is lms-type
            bool isL1 = j != (beg + size - 1) && suffix_array[j] > suffix_array[j + 1];
            //j and j + 1 is stored in the same block,
            //if (j + 1) is l-type then suffix_array[j + 1] points to the begin of the block and therefore < i
            //if (j + 1) is lms-type then suffix_array[j + 1] points to the end of the block and therefore > i
            bool isL2 = j != (beg + size - 1) && suffix_array[j] == suffix_array[j + 1] && suffix_array[j + 1] < i;
            if (!isL1 && !isL2) {
                suffix_array[i] = EMPTY;
            }
            //Also if we are sorting lms-substrings we don't need L-suffixes and LMS-suffixes
            //except the most left one
            if (sort_lms_substrings) {
                suffix_array[i] = EMPTY;
            }
        }
    }

    //now we need to shift left all the values in the block,
    //if the block still has counter
    int i = 0;
    while (i < size) {
        if (suffix_array[i] < 0 && suffix_array[i] != EMPTY) {
            //then suffix_array[i] is a counter
            int j = i;
            i = i - suffix_array[i] - 1;
            while (j < i) {
                suffix_array[j] = suffix_array[j + 1];
                j++;
            }
            suffix_array[j] = EMPTY;
        }
        i++;
    }
}

void SuffixSort::inducedSort1S(std::vector<int> &suffix_array,
                               int beg, int size, bool sort_lms_substrings) {
    for (int i = size - 1; i >= 0; i--) {
        int new_i = i;
        int j = suffix_array[i];
        if (j < 0) {
            continue;
        }
        if (j == beg) {
            if (sort_lms_substrings) {
                //we never delete the first element, but it is not lms
                suffix_array[i] = EMPTY;
            }
            continue;
        }
        if (suffix_array[j - 1] < suffix_array[j] ||
            (suffix_array[j - 1] == suffix_array[j] && suffix_array[j - 1] > i)) {
            //then (j - 1) is s-suffix and we need to find position for it
            //suffix_array[i] points to the end of it's block
            int block_end = suffix_array[j - 1];
            if (suffix_array[block_end] == EMPTY) {
                if (suffix_array[block_end - 1] == EMPTY) {
                    suffix_array[block_end - 1] = j - 1;
                    //-suffix_array[block_end] shows how many elements(+1) have been already stored
                    suffix_array[block_end] = -2;
                }
                else {
                    //j - 1 is the only element in it's block
                    suffix_array[block_end] = j - 1;
                }
            }
            else if(suffix_array[block_end] != EMPTY && suffix_array[block_end] < 0) {
                //points to the free space in the block
                int k = block_end + suffix_array[block_end]; //in fact the value subtracts
                if (suffix_array[k] == EMPTY) {
                    suffix_array[k] = j - 1;
                    //increase the element counter
                    suffix_array[block_end]--;
                } else {
                    //block is full, we need to shift the elements to the right
                    int prev_elem = EMPTY;
                    for (int u = k + 1; u <= block_end; u++) {
                        std::swap(prev_elem, suffix_array[u]);
                    }
                    if (suffix_array[j - 1] == suffix_array[j]) {
                        //in this case index j(=suffix_array[i]) is also s-type
                        //and is stored in the same block and we've shifted all the block to the left
                        //so the we don't need to increment `i` in the for cycle
                        new_i = i + 1;
                    }
                    suffix_array[k + 1] = j - 1;
                }
            } else { //suffix_array[block_end] >= 0
                //then suffix_array[block_end] is value from another block and
                //we should shift that block to the right
                int k = block_end;
                //while suffix_array[k] is not counter
                int last_value = EMPTY;
                while (suffix_array[k] >= 0 || suffix_array[k] == EMPTY) {
                    std::swap(suffix_array[k], last_value);
                    k++;
                }
                suffix_array[k] = last_value;
                if (k > i) {
                    //suffix_array[j - 1] <= suffix_array[j] and then `i` > suffix_array[j - 1]
                    //and if k > i then we've shifted i to the right
                    new_i = i + 1;
                }

                //then perform the same action as in the first case
                if (suffix_array[block_end - 1] == EMPTY) {
                    suffix_array[block_end - 1] = j - 1;
                    //-suffix_array[block_end] shows how many elements(+1) have been already stored
                    suffix_array[block_end] = -2;
                }
                else {
                    //j - 1 is the only element in it's block
                    suffix_array[block_end] = j - 1;
                }
            }
            i = new_i;

            //if we are sorting lms-substrings we need to leave only lms indexes
            if (sort_lms_substrings) {
                suffix_array[i] = EMPTY;
            }
        }
    }
    //now we need to shift right all the values in the block,
    //if the block still has counter
    int i = size - 1;
    while (i >= 0) {
        if (suffix_array[i] < 0 && suffix_array[i] != EMPTY) {
            //then suffix_array[i] is a counter
            int j = i;
            i = i + suffix_array[i] + 1;
            while (j > i) {
                suffix_array[j] = suffix_array[j - 1];
                j--;
            }
            suffix_array[j] = EMPTY;
        }
        i--;
    }

    //we delete the last lms index earlier
    suffix_array[0] = beg + size - 1;

    if (sort_lms_substrings) {
        //now indexes that corresponds to the lms-suffixes are stored
        //in `suffix_array` in the right order,
        //and other values is set to EMPTY
        //we want to move all the lms-suffixes to the front
        int j = 0;
        for (int i = 0; i < size; i++) {
            if (suffix_array[i] != EMPTY) {
                std::swap(suffix_array[j++], suffix_array[i]);
            }
        }
    }
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
            for (; cur_lms <= cur_lms_end && prev_lms <= prev_lms_end; cur_lms++, prev_lms++) {
                if (suffix_array[cur_lms] != suffix_array[prev_lms]) {
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

void SuffixSort::putLMSToBegin1(std::vector <int> &suffix_array, int lms_n, int beg, int size) {
    //at first store all lms-suffixes in the `suffix_array` from beg - lms_n to beg
    int n = size;
    //current position for lms-suffix
    int last_free = beg - 1;
    int last_type = 0, cur_type;
    for (int i = (int) beg + size - 2; i >= beg; i--) {
        if (suffix_array[i] == suffix_array[i + 1]) {
            cur_type = last_type;
        } else {
            if (suffix_array[i] < suffix_array[i + 1]) {
                cur_type = 0;
            } else {
                cur_type = 1;
            }
        }
        if (cur_type == 1 && last_type == 0) {
            suffix_array[last_free--] = i + 1;
        }
        last_type = cur_type;
    }

    //put all lms-suffixes in the right places in `suffix_array`
    for (int i = 0; i < lms_n; i++) {
        //let j = suffix_array[i]
        //suffix_array[j] -- index of the lms-suffix number j
        int j = suffix_array[i];
        suffix_array[i] = suffix_array[j];
        suffix_array[j] = EMPTY;
    }
}