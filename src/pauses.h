#ifndef PAUSES_H
#define PAUSES_H

#define MAX_PAUSES 10 // Максимальное количество пауз

// Структура для описания паузы
struct Pause {
    float temperature; // Температура
    float hysteresis;  // Гистерезис
    int time;          // Время в минутах
};

// Объявление массива пауз и их количества
extern Pause pauses[MAX_PAUSES];
extern int pauseCount;

// Функция инициализации пауз
void initPauses();

#endif // PAUSES_H
