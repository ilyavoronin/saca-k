#include "SuffixSort.h"




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
                last_type = cur_type;
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
            else if(suffix_array[block_end] < 0) {
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
                suffix_array[k] = last_value;

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
            else if (suffix_array[block_begin] < 0) {
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
            else if(suffix_array[block_end] < 0) {
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