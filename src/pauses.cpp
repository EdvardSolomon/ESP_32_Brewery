#include "pauses.h"

// Глобальная переменная для хранения пауз
Pause pauses[MAX_PAUSES];
int pauseCount = 1; // Текущее количество пауз

// Инициализация пауз значениями по умолчанию
void initPauses() {
    for (int i = 0; i < MAX_PAUSES; i++) {
        pauses[i].temperature = 50.0; // Температура по умолчанию
        pauses[i].hysteresis = 3.0;   // Гистерезис по умолчанию
        pauses[i].time = 10;          // Время по умолчанию
    }
}
