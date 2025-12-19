#include "../include/block_sort.h"
#include <algorithm>
#include <cmath>
#include <cstddef>

using namespace std;

namespace sort {

// Вспомогательная сортировка вставками для подмассива [l, r].
// Используется для сортировки небольших блоков перед слиянием.
static void insertion_sort(vector<int>& a, size_t l, size_t r) {
    for (size_t i = l + 1; i <= r; ++i) {
        int key = a[i];
        std::ptrdiff_t j = (std::ptrdiff_t)i - 1;
        while (j >= (std::ptrdiff_t)l && a[(size_t)j] > key) {
            a[(size_t)j + 1] = a[(size_t)j];
            --j;
        }
        a[(size_t)j + 1] = key;
    }
}

// Основной метод сортировки: разбиваем массив на блоки размера ~sqrt(n),
// сортируем блоки вставками, затем сливаем блоки с помощью inplace_merge.
void BlockSorter::sort(vector<int>& data) {
    size_t n = data.size();
    if (n <= 1) return;

    // Выбор размера блока k ≈ sqrt(n)
    size_t k = max((size_t)1, (size_t)floor(sqrt((double)n)));

    // Сортируем каждый блок вставками
    for (size_t i = 0; i < n; i += k) {
        size_t l = i;
        size_t r = min(n - 1, i + k - 1);
        insertion_sort(data, l, r);
    }

    // Сливаем блоки попарно с помощью inplace_merge
    for (size_t width = k; width < n; width *= 2) {
        for (size_t i = 0; i + width < n; i += 2 * width) {
            size_t mid = i + width;
            size_t right = min(n, i + 2 * width);
            inplace_merge(data.begin() + i, data.begin() + mid, data.begin() + right);
        }
    }
}

}
