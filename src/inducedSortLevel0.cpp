#include <SuffixSort.h>



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
            int pos = block_begin[symb] + cur_shift[symb];
            suffix_array[pos] = j - 1;
            cur_shift[symb]++;
            if (sort_lms_substrings) {
                //we don't need this value anymore
                suffix_array[i] = EMPTY;
                if (j - 1 == 0) {
                    suffix_array[pos] = EMPTY;
                }
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


void SuffixSort::putLMSToBegin0(std::vector <int> &data, std::vector <int> &suffix_array, int lms_n) {
    //at first store all lms-suffixes in the `suffix_array` from (n - lms_n) to n
    int n = suffix_array.size();
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