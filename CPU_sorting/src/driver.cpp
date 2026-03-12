#include <algorithm>
#include <iostream>
#include <vector>
// #include <CL/opencl.hpp>

#include "bitonic_sort.hpp"

int main() {
    size_t num_elem = 0;
    std::cin >> num_elem;

    std::vector<double> vec(num_elem);
    for(auto&& elt : vec) {
        std::cin >> elt;
    }

    std::vector copy_vec1 = vec;
    std::vector copy_vec2 = vec;
    
    std::cout << std::endl << "Original vector" << std::endl;
    for(auto&& elt : vec) {
        std::cout << elt << " ";
    }

    std::sort(copy_vec1.begin(), copy_vec1.end(), std::less());
    bitsort::bitonic_sort(copy_vec2, std::less());
    
    std::cout << "\n\nSorted vector by std::sort" << std::endl;
    for(auto&& elt : copy_vec1) {
        std::cout << elt << " ";
    }
    std::cout << std::endl;

    std::cout << "\n\nSorted vector by bitonic_sort" << std::endl;
    for(auto&& elt : copy_vec2) {
        std::cout << elt << " ";
    }
    std::cout << std::endl;

}
