#include "../include/block_sort.h"
#include <cassert>
#include <vector>
#include <iostream>

using sort::BlockSorter;
using namespace std;

static bool is_sorted(const vector<int>& v) {
    for (size_t i = 1; i < v.size(); ++i) if (v[i-1] > v[i]) return false;
    return true;
}

int main() {
    {
        vector<int> v = {5,4,3,2,1};
        BlockSorter::sort(v);
        assert(is_sorted(v));
    }
    {
        vector<int> v = {};
        BlockSorter::sort(v);
        assert(is_sorted(v));
    }
    {
        vector<int> v = {1};
        BlockSorter::sort(v);
        assert(is_sorted(v));
    }
    {
        vector<int> v = {2,2,2,2};
        BlockSorter::sort(v);
        assert(is_sorted(v));
    }
    {
        vector<int> v(1000);
        for (int i = 0; i < 1000; ++i) v[i] = 1000 - i;
        BlockSorter::sort(v);
        assert(is_sorted(v));
    }
    cout << "All tests passed\n";
    return 0;
}
