#pragma once

#include <vector>
#include <string>

#include "block_sort.h"

// Способы ввода массива (избегаем магических чисел)
enum class InputMethod {
    Keyboard,   
    Random,    
    File,       
    Exit        
};
class App {
public:
    int run();

private:
    // Методы ввода/вывода
    std::vector<int> readFromFile(const std::string& path) const;
    void writeToFile(const std::string& path,
        const std::vector<int>& original,
        const std::vector<int>& sorted) const;
    void printVector(const std::vector<int>& v) const;
    std::vector<int> generateRandomData(int n) const;
};
