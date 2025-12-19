#pragma once

// Платформенные вещи, связанные с консолью и локалью.
// Здесь централизовано устанавливается кодировка консоли при запуске в main.

#include <windows.h>

inline void enableConsoleUtf8() {
    // Устанавливаем кодовую страницу консоли в UTF-8
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
}
