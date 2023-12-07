#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef enum {
    SUCCESS = 0,
    INCORRECT_INPUT,
    OVERFLOW_ERROR,
    MALLOC_ERROR,
    FILE_OPENING_ERROR,
    FILE_READING_ERROR,
    UNKNOWN_ERROR
} ErrorCode;

static const char* errorMessages[] = {
    "Всё хорошо, можно идти пить чай ☕",
    "Некорректный ввод, попробуйте ещё раз 🤨",
    "Произошло переполнение, ой 🤯",
    "Проблемы с выделением памяти, грустно 😐",
    "Не удалось открыть файл, грустно 😥",
    "Файл прочитан не полностью, грустно 😿",
    "Неизвестная ошибка, что-то пошло не так 🫢"
};

#define DEBUG 0

int hasFinalRepresentation(double fraction, int base) {
    if (fraction <= 0 || fraction >= 1 || base < 2)
        return -1;

    int upperLimit = 32;
    int i = 0;
    while (fraction != (int)fraction) {
        ++i;
        if (i >= upperLimit)
            break;
        // fraction = (fraction + (fraction*base)/base)/2;
        fraction *= base;

        if (DEBUG)
            printf("       %.20lf\n", fraction);

        if (fraction >= 1e9) {
            // слишком много попыток для данной базы
            break;
        }
    }   

    double epsilon = 0.0001;

    if (DEBUG) {
        printf("       %lf\n", epsilon + fraction);
        printf("       %lf\n", fmod(fraction, 1.0));
        printf("       %lf\n", fmod(fraction + epsilon, 1.0));
    }

    
    if (fmod(fraction, 1.0) <= epsilon || fmod(fraction + epsilon, 1.0) <= epsilon)
        return 1;
    if (fraction != (int)fraction)
        return 0;
    return 1;
}

ErrorCode checkForFinalRepresentation(int base, int parCnt, ...) {
    if (base < 2 || parCnt < 1) 
        return INCORRECT_INPUT;

    va_list fracs;
    va_start(fracs, parCnt);
    
    for (int i = 0; i < parCnt; ++i) {
        double fraction = va_arg(fracs, double);
        int res = hasFinalRepresentation(fraction, base);
        if (res == -1) 
            printf("Число %.15lf не подходит под формат ввода\n", fraction);
        else if (res == 1)
            printf("Число %.15lf имеет конечное представление в (%d)\n", fraction, base);
        else
            printf("Число %.15lf не имеет конечное представление в (%d)\n", fraction, base);
        fflush(stdout);
    }

    va_end(fracs);
    return SUCCESS;
}



int main() {
    checkForFinalRepresentation(10, 2, 1.0/1000000000000, 1.00001/1000000000);
    checkForFinalRepresentation(2, 2, 1.0/4, 1.0/3, 10.0/3);
    checkForFinalRepresentation(3, 1, 1.0/3);
    checkForFinalRepresentation(16, 1, 1.0/16);
    checkForFinalRepresentation(4, 1, 1.0/40);

    fflush(stdout);
    
    return SUCCESS;
}