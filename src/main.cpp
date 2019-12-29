#include <iostream>
#include <fstream>
#include <ctime>

#include "SuffixSort.h"

std::string PATH = "C:\\Users\\voron\\Documents\\prog\\projects\\saca-k\\files\\";

int main() {
    std::ifstream in;
    in.open(PATH + "test.txt", std::ios::binary);
    std::vector <int> data, sorted_suffixes;
    char c;
    while ((c = in.get()) != -1) {
        data.push_back(c);
    }
    std::cout << "data size: " << data.size() << "\n";
    data.push_back(0);
    double begin_time = clock();
    SuffixSort().sort(data, sorted_suffixes);
    double work_time = (clock() - begin_time) / CLOCKS_PER_SEC;
    std::cout << "sorted in " << work_time << " seconds\n";
    return 0;
}
