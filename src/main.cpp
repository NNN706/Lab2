#include "../include/app.h"
#include <iostream>
#include <clocale>
#include <windows.h>

int main() {
    // Установить локаль по умолчанию ОС (без изменения кодовой страницы через WinAPI)
    setlocale(LC_ALL, "");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    // Приветствие студента и информация о работе
    std::cout << "Иванов Руслан, группа 443, Лабораторная работа 2, вариант 21\n";
    std::cout << "Консольное приложение — блочная сортировка\n";

    App app;
    return app.run();
}
