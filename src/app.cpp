#include "../include/app.h"
#include <iostream>
#include <fstream>
#include <random>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <limits>
#include <ctime>
#include "../include/block_sort.h"

using namespace std;
using sort::BlockSorter;

std::vector<int> App::generateRandomData(int n) const {
    std::vector<int> out;
    out.reserve(n);

    static bool seeded = false;
    if (!seeded) {
        std::srand((unsigned)std::time(nullptr));
        seeded = true;
    }

    for (int i = 0; i < n; ++i) {
        int sign = (std::rand() % 2 == 0) ? 1 : -1;
        int threeDigit = std::rand() % 900 + 100; // 100..999
        int frac = std::rand() % 100; // 0..99 for fractional part
        int value = sign * (threeDigit * 100 + frac); // store as integer = value*100
        out.push_back(value);
    }
    return out;
}

// Валидация: чтение целого числа в диапазоне
static int readIntInRange(const string& prompt, int minVal, int maxVal) {
    while (true) {
        cout << prompt;
        int x;
        if (!(cin >> x)) {
            cout << "Ошибка ввода: ожидается целое число. Попробуйте снова.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        // Проверяем, что введено именно целое число и ничего лишнего
        char next = cin.peek();
        if (next != '\n' && next != EOF) {
            // Во входе есть лишние символы
            cout << "Ошибка: введены лишние символы. Попробуйте снова.\n";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (x < minVal || x > maxVal) {
            cout << "Ошибка: число должно быть в диапазоне [" << minVal << ", " << maxVal << "]\n";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        // Очищаем буфер от символа новой строки
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return x;
    }
}
// Валидация: чтение положительного целого (количество элементов)
static int readPositiveInt(const string& prompt) {
    while (true) {
        cout << prompt;
        int x;
        if (!(cin >> x)) {
            cout << "Ошибка ввода: ожидается целое число. Попробуйте снова.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        if (x <= 0) {
            cout << "Ошибка: введите положительное число.\n";
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return x;
    }
}

// Валидация ответа yes/no
static bool askYesNo(const string& prompt) {
    while (true) {
        cout << prompt;
        string s;
        if (!(cin >> s)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        // нормализуем
        if (s.size() == 0) continue;
        char c = s[0];
        if (c == 'y' || c == 'Y' || c == 'д' || c == 'Д') return true;
        if (c == 'n' || c == 'N' || c == 'н' || c == 'Н') return false;
        cout << "Ошибка: ожидается 'y'/'n' (или рус. 'д'/'н'). Попробуйте снова.\n";
    }
}

// Получить базовое имя файла (без пути и расширения)
static string getBaseName(const string& path) {
    size_t pos = path.find_last_of("/\\");
    string name = (pos == string::npos) ? path : path.substr(pos + 1);
    size_t dot = name.find_last_of('.');
    if (dot != string::npos) name = name.substr(0, dot);
    return name;
}

// Проверка, является ли имя файла зарезервированным в Windows
static bool isReservedWindowsName(const string& path) {
    string name = getBaseName(path);
    if (name.empty()) return true;
    // убрать пробелы в конце и точки
    while (!name.empty() && (name.back() == ' ' || name.back() == '.')) name.pop_back();
    string up;
    up.reserve(name.size());
    for (char c : name) up.push_back((char)toupper((unsigned char)c));
    static const vector<string> reserved = {
        "CON","PRN","AUX","NUL",
        "COM1","COM2","COM3","COM4","COM5","COM6","COM7","COM8","COM9",
        "LPT1","LPT2","LPT3","LPT4","LPT5","LPT6","LPT7","LPT8","LPT9"
    };
    for (const auto& r : reserved) if (up == r) return true;
    return false;
}

int App::run() {
    while (true) {
        cout << "Выберите способ ввода массива:\n";
        cout << "1) Ввод с клавиатуры\n2) Генерация случайного массива\n3) Загрузка из файла\n4) Выход\n";
        int maxChoice = static_cast<int>(InputMethod::Exit);
        int choice = readIntInRange("Ваш выбор: ", static_cast<int>(InputMethod::Keyboard), maxChoice);
        InputMethod method = static_cast<InputMethod>(choice);
        if (method == InputMethod::Exit) {
            cout << "Выход.\n";
            break;
        }

        vector<int> data;
        if (method == InputMethod::Keyboard) {
            cout << "Введите числа через пробел (можно дробные), завершите ввод пустой строкой:\n";
            string line;
            // Очищаем буфер перед началом ввода
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            while (true) {
                cout << "> ";
                if (!std::getline(cin, line)) break; // EOF
                if (line.empty()) break; // завершение ввода по пустой строке
                stringstream ss(line);
                double xd;
                bool any = false;
                while (ss >> xd) {
                    int scaled = (int)lround(xd * 100.0);
                    data.push_back(scaled);
                    any = true;
                }
                if (!any) cout << "В строке не найдено чисел, попробуйте ещё раз или нажмите Enter для завершения.\n";
            }
        }
        else if (method == InputMethod::Random) {
            int n = readPositiveInt("Введите количество элементов: ");
            data = generateRandomData(n);
        }
        else if (method == InputMethod::File) {

            cout << "Введите путь к файлу: ";
            string path;
            if (!getline(cin, path)) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Ошибка чтения пути.\n";
                continue;
            }

            // Удаляем пробелы в начале и конце строки
            path.erase(0, path.find_first_not_of(" \t\n\r\f\v"));
            path.erase(path.find_last_not_of(" \t\n\r\f\v") + 1);

            if (path.empty()) {
                cout << "Путь к файлу не может быть пустым.\n";
                continue;
            }

            // Открываем файл
            ifstream in(path);
            if (!in) {
                cout << "Не удалось открыть файл: " << path << "\n";
                continue;
            }

            // Читаем данные из файла
            data = readFromFile(path);
            if (data.empty()) {
                cout << "Файл пуст или не удалось загрузить числовые данные.\n";
                continue;
            }
        }
        else {
            cout << "Неверный выбор\n";
            continue;
        }

        // Если массив пустой, возвращаемся в меню
        if (data.empty()) {
            cout << "Массив пуст.\n";
            continue;
        }

        cout << "Исходный массив:\n";
        printVector(data);

        // Выполнение сортировки
        BlockSorter::sort(data);

        cout << "Отсортированный массив:\n";
        printVector(data);

        bool save = askYesNo("Сохранить результат в файл? (y/n): ");
        if (save) {
            string outpath;
            // Очищаем буфер перед вводом пути для сохранения
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            while (true) {
                cout << "Введите путь для сохранения: ";
                if (!getline(cin, outpath)) {
                    cout << "Ошибка чтения пути.\n";
                    break;
                }

                // Удаляем пробелы в начале и конце строки
                outpath.erase(0, outpath.find_first_not_of(" \t\n\r\f\v"));
                outpath.erase(outpath.find_last_not_of(" \t\n\r\f\v") + 1);

                if (outpath.empty()) {
                    cout << "Путь для сохранения не может быть пустым.\n";
                    continue;
                }

                if (isReservedWindowsName(outpath)) {
                    cout << "Ошибка: имя файла зарезервировано системой. Выберите другое имя.\n";
                    continue;
                }

                // попытка открыть для записи
                ofstream test(outpath, ios::out);
                if (!test) {
                    cout << "Не удалось открыть файл для записи: " << outpath << "\n";
                    continue;
                }
                test.close();

                // если всё OK, записываем
                writeToFile(outpath, data);
                cout << "Файл сохранён.\n";
                break;
            }
        }

        // После обработки пункта меню возвращаемся к меню
        cout << "\n";
    }

    return 0;
}
std::vector<int> App::readFromFile(const std::string& path) const {
    std::vector<int> v;
    std::ifstream in(path);
    if (!in) return v;
    double xd;
    while (in >> xd) {
        int scaled = (int)std::lround(xd * 100.0);
        v.push_back(scaled);
    }
    return v;
}

void App::writeToFile(const std::string& path, const std::vector<int>& v) const {
    std::ofstream out(path);
    for (size_t i = 0; i < v.size(); ++i) {
        double val = v[i] / 100.0;
        out << std::fixed << std::setprecision(2) << val;
        if (i + 1 < v.size()) out << " ";
    }
}

void App::printVector(const std::vector<int>& v) const {
    for (size_t i = 0; i < v.size(); ++i) {
        if (i) std::cout << " ";
        double val = v[i] / 100.0;
        std::cout << std::fixed << std::setprecision(2) << val;
    }
    std::cout << '\n';
}