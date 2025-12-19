#pragma once

#include <vector>

// Блочная сортировка: модуль сортировки целых чисел.
// Реализация в отдельном классе BlockSorter.
// Не использует std::sort для основной сортировки (использует inplace_merge для слияния).

namespace sort {

class BlockSorter {
public:
    // Сортирует вектор целых чисел на месте.
    // Параметр:
    //   data - ссылка на вектор, который будет отсортирован.
    static void sort(std::vector<int>& data);
};

} // namespace sort
