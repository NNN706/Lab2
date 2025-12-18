#pragma once

#include <vector>

// Block sort (also called block merge or "library sort" variant) implementation
// Provides sorting of integer vectors without using std::sort.

namespace sort {

class BlockSorter {
public:
    // Sort the vector in-place
    static void sort(std::vector<int>& data);
};

} // namespace sort
