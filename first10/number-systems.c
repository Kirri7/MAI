#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include "number-systems.h"

int defineBase(const char* str) {
    if (*str == '-' || *str == '+') {
        ++str;
    }
    int base = 0;
    if (*str == '0') {
        ++str;
        if (*str == 'x' || *str == 'X') {
            base = 16;
        } else {
            base = 8;
        }
    } else {
        base = 10;
    }
    return base;
}

int fromDecimal(int decimal, int base, char** res) {
    if (2 > base || base > 36) {
        printf("Это не база: она от 2 до 36 🗿\n");
        return 1;
    }

    int maxLen = 0;
    int sign = 1;
    if (decimal < 0) {
        ++maxLen; // для минуса
        decimal *= -1;
        sign = -1;
    }
    // Макс длина строки в новой системе счисления через формулу logарифма
    maxLen += (decimal == 0) ? 1 : (int)(log(decimal) / log(base)) + 1;
    ++maxLen; // для терминатора

    // Динамическая строка для результата
    char* output = (char*)malloc(maxLen * sizeof(char));

    if (output == NULL) {
        printf("Память как у рыбки - не выделилась, грустно 🐠\n");
        return 1;
    }

    int i = maxLen - 2; // начнём запись с конца строки
    while (decimal > 0) {
        int digit = decimal % base;
        output[i--] = (digit < 10) ? (char)(digit + '0') : (char)(digit - 10 + 'A');
        decimal /= base;
    }

    if (i == maxLen - 2) {
        // число = 0
        output[i--] = '0';
    }

    if (sign == -1)
        output[i--] = '-';

    output[maxLen - 1] = '\0';
    *res = output;

    return 0; // успех
}

int handmadeStrtoi(const char* str, char** endptr, int base, int* ovrflw) {
    if (2 > base || base > 36) {
        printf("Это не база: она от 2 до 36 🗿\n");
        *ovrflw = 1;
        return 0;
    }

    if (str == NULL) {
        printf("handmadeStrtol(NULL, ..) 🫢\n");
        *ovrflw = 1;
        return 0;
    }
    *ovrflw = 0;
    
    while (isspace(*str)) {
        ++str;
    }

    int sign = 1;
    if (*str == '-') {
        sign = -1;
        ++str;
    } else if (*str == '+') {
        ++str;
    }

    int result = 0;
    while (isdigit(*str) || (*str >= 'a' && *str <= 'z') || (*str >= 'A' && *str <= 'Z')) {
        int digit;
        if (isdigit(*str)) {
            digit = *str - '0';
        } else {
            digit = tolower(*str) - 'a' + 10;
        }

        if (digit >= base) {
            break; // такая себе циферка
        }

        int ovrflwCheck = result;
        result *= base; // сдвиг на разряд
        result += digit;
        if (result < ovrflwCheck) {
            *ovrflw = 1;
            result = ovrflwCheck;
            break;
        }
        ++str;
    }

    if (endptr != NULL) {
        *endptr = (char*)str;
    }

    return sign * result;
}

int toDecimal(const char* str, int base, int* res) {
    char* endPtr;
    int ovrflw = 0;
    //long decimal = strtol(str, &endPtr, base);
    int decimal = handmadeStrtoi(str, &endPtr, base, &ovrflw);

    if (ovrflw) {
        printf("Переполнение при преобразовании строки в число, грустно 😿\n");
        return 1;
    }

    if (*endPtr != '\0') {
        printf("Ошибка при преобразовании строки в число, грустно 😿\n");
        return 1;
    }

    *res = decimal;
    return 0;
}

// int fromDecimal(char* decimal, int dSize,  int base, char** res); -> не вариант, операции от int только в распоряжении