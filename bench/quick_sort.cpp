#include <qpl/qpl.h>
#include <numeric>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <thread>
#include <limits>
#include <chrono>
#include <future>
#include <queue>

#include "sort.hpp"
#include "utils.hpp"


int main(int argc, char** argv) {
    qpl_path_t execution_path = qpl_path_hardware;
    if (parse_execution_path(argc, argv, &execution_path, 0) != 0) { return 1; }

    std::vector<uint32_t> data(1024 * 1024 * 16);
    std::iota(data.begin(), data.end(), 1);
    std::reverse(data.begin(), data.end());

    std::vector<uint32_t> std_sorted(1024 * 1024 * 16);
    std::iota(std_sorted.begin(), std_sorted.end(), 1);
    std::reverse(std_sorted.begin(), std_sorted.end());

    std::vector<uint32_t> sorted;
    auto start = std::chrono::high_resolution_clock::now();
    chunked_iaa_quicksort(execution_path, data, sorted, 384);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed_std = end_std - start_std;
    std::cout << "chunked::iaa_quick_sort Time: " << elapsed_std.count() << " ms\n";

    
    auto start_std = std::chrono::high_resolution_clock::now();
    std::sort(std_sorted.begin(), std_sorted.end());
    auto end_std = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed_std = end_std - start_std;
    std::cout << "std::sort Time: " << elapsed_std.count() << " ms\n";


    auto start_std = std::chrono::high_resolution_clock::now();
    std::qsort(
    data.data(),                        
    data.size(),                        
    sizeof(decltype(std_sorted)::value_type), 
    [](const void* x, const void* y) -> int { 
        const int arg1 = *static_cast<const int*>(x);
        const int arg2 = *static_cast<const int*>(y);
        const auto cmp = arg1 <=> arg2;
        if (cmp < 0) return -1;
        if (cmp > 0) return 1;
        return 0;
    }
);
    auto end_std = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed_std = end_std - start_std;
    std::cout << "std::qsort Time: " << elapsed_std.count() << " ms\n";
    
    return 0;
}
