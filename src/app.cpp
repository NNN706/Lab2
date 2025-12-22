#include "../include/app.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <sstream>
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
        // Генерация целых чисел в диапазоне [-1000, 1000]
        int sign = (std::rand() % 2 == 0) ? 1 : -1;
        int value = sign * (std::rand() % 1001); // 0..1000 со знаком
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
        cout << static_cast<int>(InputMethod::Keyboard) + 1 << ") Ввод с клавиатуры\n";
        cout << static_cast<int>(InputMethod::Random) + 1 << ") Генерация случайного массива\n";
        cout << static_cast<int>(InputMethod::File) + 1 << ") Загрузка из файла\n";
        cout << static_cast<int>(InputMethod::Exit) + 1 << ") Выход\n";

        int minChoice = static_cast<int>(InputMethod::Keyboard) + 1;
        int maxChoice = static_cast<int>(InputMethod::Exit) + 1;
        int choice = readIntInRange("Ваш выбор: ", minChoice, maxChoice);

        InputMethod method = static_cast<InputMethod>(choice - 1);

        vector<int> data;

        switch (method) {
        case InputMethod::Exit:
            cout << "Выход.\n";
            return 0;

        case InputMethod::Keyboard: {
            cout << "Введите числа через пробел (допускаются дробные), завершите ввод пустой строкой:\n";
            string line;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            while (true) {
                cout << "> ";
                if (!std::getline(cin, line) || line.empty()) break;

                stringstream ss(line);
                double num;
                bool any = false;

                while (ss >> num) {
                    int scaled = static_cast<int>(std::lround(num * 100.0));
                    data.push_back(scaled);
                    any = true;
                }

                if (!any) {
                    cout << "В строке не найдено чисел, попробуйте ещё раз или нажмите Enter для завершения.\n";
                }
            }
            break;
        }

        case InputMethod::Random: {
            int n = readPositiveInt("Введите количество элементов: ");
            data = generateRandomData(n);
            break;
        }

        case InputMethod::File: {
            cout << "Введите путь к файлу: ";
            string path;


            if (!getline(cin, path)) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Ошибка чтения пути.\n";
                continue;
            }

            path.erase(0, path.find_first_not_of(" \t\n\r\f\v"));
            path.erase(path.find_last_not_of(" \t\n\r\f\v") + 1);

            if (path.empty()) {
                cout << "Путь к файлу не может быть пустым.\n";
                continue;
            }

            ifstream in(path);
            if (!in) {
                cout << "Не удалось открыть файл: " << path << "\n";
                continue;
            }

            data = readFromFile(path);
            if (data.empty()) {
                cout << "Файл пуст или не удалось загрузить числовые данные.\n";
                continue;
            }
            break;
        }

        default:
            cout << "Неверный выбор\n";
            continue;
        }

        // В методе run() после switch-case добавить:
// Если массив пустой, возвращаемся в меню
        if (data.empty()) {
            cout << "Массив пуст.\n";
            continue;
        }

        // Сохраняем оригинальный массив
        vector<int> originalData = data;

        cout << "Исходный массив (" << data.size() << " элементов):\n";
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

                // если всё OK, записываем оба массива
                writeToFile(outpath, originalData, data);
                cout << "Файл сохранён.\n";
                break;
            }
        }

        // После обработки пункта меню возвращаемся к меню
        cout << "\n";
    }
}

std::vector<int> App::readFromFile(const std::string& path) const {
    std::vector<int> v;
    std::ifstream in(path);
    if (!in) return v;

    // Читаем числа (целые или дробные)
    double num;
    while (in >> num) {
        // Умножаем на 100 и округляем для внутреннего хранения
        int scaled = static_cast<int>(std::lround(num * 100.0));
        v.push_back(scaled);
    }
    return v;
}
void App::writeToFile(const std::string& path,
    const std::vector<int>& original,
    const std::vector<int>& sorted) const {
    std::ofstream out(path);
    if (!out) {
        std::cerr << "Ошибка при открытии файла для записи: " << path << std::endl;
        return;
    }

    // Записываем заголовок и исходный массив
    out << "Исходный массив (" << original.size() << " элементов):\n";
    for (size_t i = 0; i < original.size(); ++i) {
        if (i) out << " ";
        double val = original[i] / 100.0;
        out << val;
    }
    out << "\n\n";

    // Записываем заголовок и отсортированный массив
    out << "Отсортированный массив:\n";
    for (size_t i = 0; i < sorted.size(); ++i) {
        if (i) out << " ";
        double val = sorted[i] / 100.0;
        out << val;
    }
    out << "\n";
}

void App::printVector(const std::vector<int>& v) const {
    for (size_t i = 0; i < v.size(); ++i) {
        if (i) std::cout << " ";
        // Конвертируем обратно в дробное число для вывода
        double val = v[i] / 100.0;
        std::cout << val;
    }
    std::cout << '\n';
}