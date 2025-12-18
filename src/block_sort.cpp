#include "../include/block_sort.h"
#include <algorithm>
#include <cmath>

using namespace std;

namespace sort {

// We'll implement a simple block sort: divide into blocks of size k, sort each block with insertion sort
// then merge blocks using std::inplace_merge (allowed as part of standard library merging utilities but
// not using std::sort as per requirement). This achieves O(n log n) merging stage with small-block insertion.

static void insertion_sort(vector<int>& a, size_t l, size_t r) {
    for (size_t i = l + 1; i <= r; ++i) {
        int key = a[i];
        ssize_t j = i - 1;
        while (j >= (ssize_t)l && a[j] > key) {
            a[j + 1] = a[j];
            --j;
        }
        a[j + 1] = key;
    }
}

void BlockSorter::sort(vector<int>& data) {
    size_t n = data.size();
    if (n <= 1) return;

    // choose block size k ~ sqrt(n)
    size_t k = max((size_t)1, (size_t)floor(sqrt((double)n)));

    // sort each block
    for (size_t i = 0; i < n; i += k) {
        size_t l = i;
        size_t r = min(n - 1, i + k - 1);
        insertion_sort(data, l, r);
    }

    // merge blocks pairwise using inplace_merge
    for (size_t width = k; width < n; width *= 2) {
        for (size_t i = 0; i + width < n; i += 2 * width) {
            size_t mid = i + width;
            size_t right = min(n, i + 2 * width);
            inplace_merge(data.begin() + i, data.begin() + mid, data.begin() + right);
        }
    }
}

} // namespace sort
