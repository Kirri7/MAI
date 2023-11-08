#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define printf(format, ...) printf(format, ##__VA_ARGS__); fflush(stdout);

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

void dumpSignedInt(int *ptr) {
    // little-endian
    unsigned char *bytePtr = (unsigned char *)ptr;

    for (int i = sizeof(int) - 1; i >= 0; --i) {
        for (int j = 7; j >= 0; --j) {
            printf("%d", (bytePtr[i] >> j) & 1);
        }
        printf(" ");
    }
}

ErrorCode intToBinOctHex(const int number, const int baseDegree) {
    if (baseDegree > 5 || baseDegree < 1)
        return INCORRECT_INPUT;
    
    if (number == 0) {
        printf("0");
        return SUCCESS;
    } else if (number < 0) {
        printf("-");
        intToBinOctHex(-1 * number, baseDegree);
        return SUCCESS;
    }

    unsigned char *bytePtr = (unsigned char *)&number;
    if (bytePtr == NULL)
        return UNKNOWN_ERROR;

    int bitsRead = 0;
    if (32 % baseDegree != 0)
        bitsRead = baseDegree - (32 % baseDegree);

    int zeroFlag = 0;
    int digit = 0;
    // little-endian notation
    for (int i = sizeof(int) - 1; i >= 0; --i) { // по байтам
        for (int j = 7; j >= 0; --j) { // по битам
            // достать текущий бит (bytePtr[i] >> j) & 1
            // a = a | 1 << n положить бит на n порядок
            ++bitsRead;
            digit = digit | (bytePtr[i] >> j & 1) << (baseDegree - bitsRead);
            if (bitsRead == baseDegree) {
                // демонстрация работы:
                // printf("\n%d -> ", digit);
                // dumpSignedInt(&digit);
                if (digit != 0 || zeroFlag != 0) 
                {
                    zeroFlag = 1;
                    char out = (digit < 10) ? (char)(digit + '0') : (char)(digit - 10 + 'A');
                    printf("%c", out);
                }
                bitsRead = 0;
                digit = 0;
            }
        }
    }
    return SUCCESS;
}

//00000000000000000000000001111011 
//br 0 -> 011 110 11...

int main() {
    int number = -123;
    int baseDegree = 4;

    switch (intToBinOctHex(number, baseDegree)) {
        case INCORRECT_INPUT:
            printf("%s\n", errorMessages[INCORRECT_INPUT]);
            return INCORRECT_INPUT;
        default:
            break;
    }
    printf("\n");
    return SUCCESS;
}

