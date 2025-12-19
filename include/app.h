#pragma once

#include <vector>
#include <string>

#include "block_sort.h"

// Способы ввода массива (избегаем магических чисел)
enum class InputMethod {
    Keyboard = 1,
    Random = 2,
    File = 3,
    Exit = 4
};

// Класс приложения: отвечает за ввод, вывод и запуск сортировки
class App {
public:
    // Запуск приложения
    int run();

private:
    // Методы ввода/вывода
    std::vector<int> readFromFile(const std::string& path) const;
    void writeToFile(const std::string& path, const std::vector<int>& v) const;
    void printVector(const std::vector<int>& v) const;

    // Вспомогательный генератор случайных чисел собственной реализации
    std::vector<int> generateRandomData(int n) const;
};
