#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <stdarg.h>

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

ErrorCode intToRoman(int num);
ErrorCode findLargestFibonacci(unsigned int n, unsigned int* prevFib, unsigned int* answer);
ErrorCode findPrevFibonacci(unsigned int fib, unsigned int n, unsigned int prev);
ErrorCode cekendorfRepresentation(unsigned int n);
ErrorCode fromDecimal(int decimal, int base, int isUpper, char** res);
int handmadeStrtoi(const char* str, char** endptr, int base, int* exitStatus);
ErrorCode toDecimal(const char* str, int base, int* res);
void dumpSignedInt(int *ptr);
void dumpUnsignedInt(unsigned int *ptr);
void dumpDouble(double *ptr);
void dumpFloat(float *ptr);
ErrorCode overfprintf(FILE *stream, const char *format, ...);

int main(int argc, char *argv[]) {

    overfprintf(stdout,"h%Ro %d lo\n", 14, 113);
    overfprintf(stdout,"32: %Zr|\n", 32);
    overfprintf(stdout,"-11: %Cv|\n", -11, 12);
    overfprintf(stdout,"35: %CV|\n", 35, 36);
    overfprintf(stdout,"-256: %CV|\n", -256, 0);
    overfprintf(stdout,"-256: %to|\n", "-256", 0);
    overfprintf(stdout,"-ZA: %TO|\n", "-ZA", 36);
    overfprintf(stdout,"2147483648: %mi\n", 2147483648);
    overfprintf(stdout,"2147483648: %mu\n", 2147483648);
    overfprintf(stdout,"2.4: %md\n", 2.4);
    overfprintf(stdout,"2.4: %mf\n", 2.4);


    return 0;
}

ErrorCode intToRoman(int num) {
    //https://www.romannumerals.org/blog/which-is-the-biggest-number-in-roman-numerals-6 -> [1,3999]
    if (num < 1 || num > 3999) {
        //printf("\nЧисло не входит в диапазон от 1 до 3999\n");
        return INCORRECT_INPUT;
    }

    char *roman[] = {"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"};
    int values[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};

    for (int i = 0; i < 13; ++i) {
        while (num >= values[i]) {
            printf("%s", roman[i]);
            num -= values[i];
        }
    }

    return SUCCESS;
}

int romanToInt(const char *roman) {
    char *symbols[] = {"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"};
    int values[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};

    int result = 0;
    int len = strlen(roman);

    for (int i = 0; i < len; ++i) {
        for (int j = 0; j < 13; ++j) {
            // сравниваем подстроки длинной с римскую цифру
            if (strncmp(roman + i, symbols[j], strlen(symbols[j])) == 0) {
                result += values[j];
                i += strlen(symbols[j]) - 1; 
                break;
            }
        }
    }

    return result;
}

ErrorCode findLargestFibonacci(unsigned int n, unsigned int* prevFib, unsigned int* answer) {
    unsigned int prev = 0, current = 1, next;

    while (current <= n) {
        next = prev + current;
        prev = current;
        current = next;
        if (prev > current) return OVERFLOW_ERROR;
    }

    *prevFib = current - prev;
    *answer = prev;
    return SUCCESS;
}

ErrorCode findPrevFibonacci(unsigned int fib, unsigned int n, unsigned int prev) {
    if (fib <= 0) {
        //return INCORRECT_INPUT;
        return SUCCESS;
    }

    int print = 0;
    if (n >= fib) {
        print = 1;
        n -= fib;
    } 

    switch (findPrevFibonacci(prev, n, (fib - prev))) {
        case INCORRECT_INPUT:
            return INCORRECT_INPUT;
        default:
            break;
    }

    if (print) {
        // printf("%d ", fib);
        printf("1 ");
        n -= fib;
    } 
    else {
        printf("0 ");
        // printf("(%d) ", fib);
    }
    return SUCCESS;
}

// цекендорфово представление чисел
ErrorCode cekendorfRepresentation(unsigned int n) {
    if (n <= 0) {
        return INCORRECT_INPUT;
    }

    unsigned int prev;
    unsigned int fib;
    switch(findLargestFibonacci(n, &prev, &fib)) {
        default:
            break;
        case OVERFLOW_ERROR:
            return OVERFLOW_ERROR;
    }

    switch (findPrevFibonacci(fib, n, prev)) {
        default:
            break;
        case INCORRECT_INPUT:
            return INCORRECT_INPUT;
    }

    printf("1");
    return SUCCESS;
}

ErrorCode fromDecimal(int decimal, int base, int isUpper, char** res) {
    if (2 > base || base > 36) {
        //printf("Это не база: она от 2 до 36 🗿\n");
        //return INCORRECT_INPUT;
        base = 10;
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
        //printf("Память как у рыбки - не выделилась, грустно 🐠\n");
        return MALLOC_ERROR;
    }

    int i = maxLen - 2; // начнём запись с конца строки
    while (decimal > 0) {
        int digit = decimal % base;
        if (isUpper)
            output[i--] = (digit < 10) ? (char)(digit + '0') : (char)(digit - 10 + 'A');
        else
            output[i--] = (digit < 10) ? (char)(digit + '0') : (char)(digit - 10 + 'a');
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
    printf("%s", output);

    return SUCCESS;
}

int handmadeStrtoi(const char* str, char** endptr, int base, int* exitStatus) {
    if (2 > base || base > 36) {
        base = 10;
    }

    if (str == NULL) {
        //printf("handmadeStrtol(NULL, ..) 🫢\n");
        *exitStatus = OVERFLOW_ERROR;
        return 0;
    }
    //*ovrflw = 0;
    
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
            *exitStatus = OVERFLOW_ERROR;
            result = ovrflwCheck;
            break;
        }
        ++str;
    }

    if (endptr != NULL) {
        *endptr = (char*)str;
    }

    *exitStatus = SUCCESS;
    return sign * result;
}

ErrorCode toDecimal(const char* str, int base, int* res) {
    char* endPtr;
    int exitStatus;
    //long decimal = strtol(str, &endPtr, base);
    int decimal = handmadeStrtoi(str, &endPtr, base, &exitStatus);

    if (exitStatus != SUCCESS) {
        //printf("Переполнение при преобразовании строки в число, грустно 😿\n");
        return OVERFLOW_ERROR;
    }

    if (*endPtr != '\0') {
        //printf("Ошибка при преобразовании строки в число, грустно 😿\n");
        return INCORRECT_INPUT;
    }

    *res = decimal;
    return SUCCESS;
}

void dumpSignedInt(int *ptr) {
    // https://www.youtube.com/watch?v=zxb8DvLUqcM
    // little-endian
    unsigned char *bytePtr = (unsigned char *)ptr;

    for (int i = sizeof(int) - 1; i >= 0; --i) {
        for (int j = 7; j >= 0; --j) {
            printf("%d", (bytePtr[i] >> j) & 1);
        }
        printf(" ");
    }
}

void dumpUnsignedInt(unsigned int *ptr) {
    unsigned char *bytePtr = (unsigned char *)ptr;

    for (int i = sizeof(unsigned int) - 1; i >= 0; --i) {
        for (int j = 7; j >= 0; --j) {
            printf("%d", (bytePtr[i] >> j) & 1);
        }
        printf(" ");
    }
}

void dumpDouble(double *ptr) {
    unsigned char *bytePtr = (unsigned char *)ptr;

    for (int i = sizeof(double) - 1; i >= 0; --i) {
        for (int j = 7; j >= 0; --j) {
            printf("%d", (bytePtr[i] >> j) & 1);
        }
        printf(" ");
    }
}

void dumpFloat(float *ptr) {
    unsigned char *bytePtr = (unsigned char *)ptr;

    for (int i = sizeof(float) - 1; i >= 0; --i) {
        for (int j = 7; j >= 0; --j) {
            printf("%d", (bytePtr[i] >> j) & 1);
        }
        printf(" ");
    }
}

ErrorCode overfprintf(FILE *stream, const char *format, ...) {
    if (format == NULL) return INCORRECT_INPUT;

    va_list argc;
    va_start(argc, format);

    const char* letterPtr = format;
    while (*letterPtr != '\0') {
        if (*letterPtr != '%') {
            printf("%c", *letterPtr);
        } 
        else if (*(letterPtr+1) != '\0' && *(letterPtr+2) != '\0')
        {
            char additFlag[] = {*(letterPtr+1), *(letterPtr+2)};
            letterPtr += 2;
            if (strcmp(additFlag, "Ro") == 0)
            {
                int num = va_arg(argc, int);
                switch (intToRoman(num)) {
                    default:
                        break;
                    case INCORRECT_INPUT:
                        return INCORRECT_INPUT;
                }
            }
            else if (strcmp(additFlag, "Zr") == 0)
            {
                unsigned int num = va_arg(argc, unsigned int);
                switch (cekendorfRepresentation(num)) {
                    default:
                        break;
                    case OVERFLOW_ERROR:
                        return OVERFLOW_ERROR;
                    case INCORRECT_INPUT:
                        return INCORRECT_INPUT;
                }
            }
            else if (strcmp(additFlag, "Cv") == 0 || strcmp(additFlag, "CV") == 0)
            {
                int num = va_arg(argc, int);
                int base = va_arg(argc, int);
                char* res; //todo
                switch (fromDecimal(num, base, isupper(additFlag[1]), &res)) {
                    default:
                        break;
                    case MALLOC_ERROR:
                        return MALLOC_ERROR;
                }
            }
            else if (strcmp(additFlag, "to") == 0 || strcmp(additFlag, "TO") == 0)
            {
                const char* num = va_arg(argc, const char*);
                int base = va_arg(argc, int);
                int res; //todo
                switch (toDecimal(num, base, &res)) {
                    default:
                        break;
                    case MALLOC_ERROR:
                        return MALLOC_ERROR;
                }
                printf("%d", res);
            }
            else if (strcmp(additFlag, "mi") == 0)
            {
                int num = va_arg(argc, int);
                dumpSignedInt(&num);
            }
            else if (strcmp(additFlag, "mu") == 0)
            {
                unsigned int num = va_arg(argc, unsigned int);
                dumpUnsignedInt(&num);
            }
            else if (strcmp(additFlag, "md") == 0)
            {
                double num = va_arg(argc, double);
                dumpDouble(&num);
            }
            else if (strcmp(additFlag, "mf") == 0)
            {
                float num = va_arg(argc, double); 
                // float num = va_arg(argc, float);
                dumpFloat(&num);
            }
            else
            {
                char additFlag2[] = {'%', *(letterPtr-1), *(letterPtr)};
                vprintf(additFlag2, argc);
            }
        }


        ++letterPtr;
    }
    va_end(argc);
    return SUCCESS;
}