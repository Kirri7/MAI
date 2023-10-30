#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
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

ErrorCode intToRoman(int num, char result[], int resLen);
ErrorCode findLargestFibonacci(unsigned int n, unsigned int* prevFib, unsigned int* answer);
ErrorCode findPrevFibonacci(unsigned int fib, unsigned int n, unsigned int prev, char result[], int resLen, int* resPos);
ErrorCode cekendorfRepresentation(unsigned int n, char result[], int resLen);
ErrorCode fromDecimal(int decimal, int base, int isUpper, char** res);
int handmadeStrtoi(const char* str, char** endptr, int base, int* exitStatus);
ErrorCode toDecimal(const char* str, int base, int* res);
void dumpfSignedInt(FILE* stream, int *ptr);
void dumpfUnsignedInt(FILE* stream, unsigned int *ptr);
void dumpfDouble(FILE* stream, double *ptr);
void dumpfFloat(FILE* stream, float *ptr);
ErrorCode overfprintf(FILE* stream, const char *format, ...);
void dumpsSignedInt(char* str, int *ptr, int* resPos);
void dumpsUnsignedInt(char* str, unsigned int *ptr, int* resPos);
void dumpsDouble(char* str, double *ptr, int* resPos);
void dumpsFloat(char* str, float *ptr, int* resPos);
ErrorCode oversprintf(char* str, const char *format, ...);

int main(int argc, char *argv[]) {
    FILE* stream = stdout;
    //FILE* stream = fopen("out.txt", "w");

    overfprintf(stream,"h%Ro %d lo\n", 14, 113);
    overfprintf(stream,"32: %Zr|\n", 32);
    overfprintf(stream,"-11: %Cv|\n", -11, 12);
    overfprintf(stream,"35: %CV|\n", 35, 36);
    overfprintf(stream,"-256: %CV|\n", -256, 0);
    overfprintf(stream,"-256: %to|\n", "-256", 0);
    overfprintf(stream,"-ZA: %TO|\n", "-ZA", 36);
    overfprintf(stream,"2147483648: %mi\n", 2147483648);
    overfprintf(stream,"2147483648: %mu\n", 2147483648);
    overfprintf(stream,"2.4: %md\n", 2.4);
    overfprintf(stream,"2.4: %mf\n", 2.4);

    char str[1000];
    for (int i = 0; i < 1000 - 1; ++i) str[i] = 'x';
    str[sizeof(str) - 1] = '\0';

    oversprintf(str,"h%Ro %d lo\n", 14, 113);
    printf("-> %s", str);
    oversprintf(str,"32: %Zr|\n", 32);
    printf("-> %s", str);
    oversprintf(str,"-11: %Cv|\n", -11, 12);
    printf("-> %s", str);
    oversprintf(str,"35: %CV|\n", 35, 36);
    printf("-> %s", str);
    oversprintf(str,"-256: %CV|\n", -256, 0);
    printf("-> %s", str);
    oversprintf(str,"-256: %to|\n", "-256", 0);
    printf("-> %s", str);
    oversprintf(str,"-ZA: %TO|\n", "-ZA", 36);
    printf("-> %s", str);
    oversprintf(str,"2147483648: %mi\n", 2147483648);
    printf("-> %s", str);
    oversprintf(str,"2147483648: %mu\n", 2147483648);
    printf("-> %s", str);
    oversprintf(str,"2.4: %md\n", 2.4);
    printf("-> %s", str);
    oversprintf(str,"2.4: %mf\n", 2.4);
    printf("-> %s", str);

    

    return 0;
}

ErrorCode intToRoman(int num, char result[], int resLen) {
    //https://www.romannumerals.org/blog/which-is-the-biggest-number-in-roman-numerals-6 -> [1,3999]
    if (num < 1 || num > 3999) {
        //printf("\nЧисло не входит в диапазон от 1 до 3999\n");
        return INCORRECT_INPUT;
    }
    int resPos = 0;

    char* roman[] = {"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"};
    int values[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};

    for (int i = 0; i < 13; ++i) {
        while (num >= values[i]) {
            //printf("%s", roman[i]);
            //sprintf(result, "%s", roman[i]);
            resPos += snprintf(result + resPos, resLen - resPos, "%s", roman[i]);
            num -= values[i];
        }
    }

    return SUCCESS;
}

int romanToInt(const char *roman) {
    char* symbols[] = {"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"};
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

ErrorCode findPrevFibonacci(unsigned int fib, unsigned int n, unsigned int prev, char result[], int resLen, int* resPos) {
    if (fib <= 0) {
        //return INCORRECT_INPUT;
        return SUCCESS;
    }
    

    int print = 0;
    if (n >= fib) {
        print = 1;
        n -= fib;
    } 

    switch (findPrevFibonacci(prev, n, (fib - prev), result, resLen, resPos)) {
        case INCORRECT_INPUT:
            return INCORRECT_INPUT;
        default:
            break;
    }

    if (print) {
        // printf("%d ", fib);
        // printf("1 ");
        // snprintf(result, strlen(result), "1 ");
        // sprintf(result, "1 ");
        *resPos += snprintf(result + *resPos, resLen - *resPos, "1 ");
        n -= fib;
    } 
    else {
        // printf("(%d) ", fib);
        // printf("0 ");
        // snprintf(result, strlen(result), "0 ");
        // sprintf(result, "0 ");
        *resPos += snprintf(result + *resPos, resLen - *resPos, "0 ");
    }
    return SUCCESS;
}

// цекендорфово представление чисел
ErrorCode cekendorfRepresentation(unsigned int n, char result[], int resLen) {
    if (n <= 0) {
        return INCORRECT_INPUT;
    }
    int resPos = 0;

    unsigned int prev;
    unsigned int fib;
    switch(findLargestFibonacci(n, &prev, &fib)) {
        default:
            break;
        case OVERFLOW_ERROR:
            return OVERFLOW_ERROR;
    }

    switch (findPrevFibonacci(fib, n, prev, result, resLen, &resPos)) {
        default:
            break;
        case INCORRECT_INPUT:
            return INCORRECT_INPUT;
    }

    // printf("1");
    //snprintf(result, strlen(result), "1 ");
    // sprintf(result, "1 ");
    resPos += snprintf(result + resPos, resLen - resPos, "1 ");
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
    //printf("%s", output);
    //free(output);

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

void dumpfSignedInt(FILE* stream, int *ptr) {
    // https://www.youtube.com/watch?v=zxb8DvLUqcM
    // little-endian
    unsigned char *bytePtr = (unsigned char *)ptr;

    for (int i = sizeof(int) - 1; i >= 0; --i) {
        for (int j = 7; j >= 0; --j) {
            fprintf(stream, "%d", (bytePtr[i] >> j) & 1);
        }
        fprintf(stream, " ");
    }
}

void dumpfUnsignedInt(FILE* stream, unsigned int *ptr) {
    unsigned char *bytePtr = (unsigned char *)ptr;

    for (int i = sizeof(unsigned int) - 1; i >= 0; --i) {
        for (int j = 7; j >= 0; --j) {
            fprintf(stream, "%d", (bytePtr[i] >> j) & 1);
        }
        fprintf(stream, " ");
    }
}

void dumpfDouble(FILE* stream, double *ptr) {
    unsigned char *bytePtr = (unsigned char *)ptr;

    for (int i = sizeof(double) - 1; i >= 0; --i) {
        for (int j = 7; j >= 0; --j) {
            fprintf(stream, "%d", (bytePtr[i] >> j) & 1);
        }
        fprintf(stream, " ");
    }
}

void dumpfFloat(FILE* stream, float *ptr) {
    unsigned char *bytePtr = (unsigned char *)ptr;

    for (int i = sizeof(float) - 1; i >= 0; --i) {
        for (int j = 7; j >= 0; --j) {
            fprintf(stream, "%d", (bytePtr[i] >> j) & 1);
        }
        fprintf(stream, " ");
    }
}

ErrorCode overfprintf(FILE* stream, const char *format, ...) {
    if (format == NULL) return INCORRECT_INPUT;

    va_list argc;
    va_start(argc, format);

    const char* letterPtr = format;
    while (*letterPtr != '\0') {
        if (*letterPtr != '%') {
            fprintf(stream,"%c", *letterPtr);
        } 
        else if (*(letterPtr+1) != '\0' && *(letterPtr+2) != '\0')
        {
            char additFlag[] = {*(letterPtr+1), *(letterPtr+2), '\0'};
            letterPtr += 2;
            const int resLen = 50;
            if (strcmp(additFlag, "Ro") == 0)
            {
                int num = va_arg(argc, int);
                char result[resLen];
                for (int i = 0; i < resLen - 1; ++i) result[i] = 'x';
                result[sizeof(result) - 1] = '\0';

                switch (intToRoman(num, result, resLen)) {
                    default:
                        break;
                    case INCORRECT_INPUT:
                        va_end(argc);
                        return INCORRECT_INPUT;
                }
                fprintf(stream, "%s", result);
            }
            else if (strcmp(additFlag, "Zr") == 0)
            {
                unsigned int num = va_arg(argc, unsigned int);
                char result[resLen];
                for (int i = 0; i < resLen - 1; ++i) result[i] = 'x';
                result[sizeof(result) - 1] = '\0';

                switch (cekendorfRepresentation(num, result, resLen)) {
                    default:
                        break;
                    case OVERFLOW_ERROR:
                        va_end(argc);
                        return OVERFLOW_ERROR;
                    case INCORRECT_INPUT:
                        va_end(argc);
                        return INCORRECT_INPUT;
                }
                fprintf(stream, "%s", result);
            }
            else if (strcmp(additFlag, "Cv") == 0 || strcmp(additFlag, "CV") == 0)
            {
                int num = va_arg(argc, int);
                int base = va_arg(argc, int);
                char* result;

                switch (fromDecimal(num, base, isupper(additFlag[1]), &result)) {
                    default:
                        break;
                    case MALLOC_ERROR:
                        va_end(argc);
                        return MALLOC_ERROR;
                }
                fprintf(stream, "%s", result);
                free(result);
            }
            else if (strcmp(additFlag, "to") == 0 || strcmp(additFlag, "TO") == 0)
            {
                const char* num = va_arg(argc, const char*);
                int base = va_arg(argc, int);
                int res;

                switch (toDecimal(num, base, &res)) {
                    default:
                        break;
                    case MALLOC_ERROR:
                        va_end(argc);
                        return MALLOC_ERROR;
                }
                fprintf(stream,"%d", res);
            }
            else if (strcmp(additFlag, "mi") == 0)
            {
                int num = va_arg(argc, int);
                dumpfSignedInt(stream, &num);
            }
            else if (strcmp(additFlag, "mu") == 0)
            {
                unsigned int num = va_arg(argc, unsigned int);
                dumpfUnsignedInt(stream, &num);
            }
            else if (strcmp(additFlag, "md") == 0)
            {
                double num = va_arg(argc, double);
                dumpfDouble(stream, &num);
            }
            else if (strcmp(additFlag, "mf") == 0)
            {
                float num = va_arg(argc, double); 
                // float num = va_arg(argc, float);
                dumpfFloat(stream, &num);
            }
            else
            {
                char additFlag2[] = {'%', *(letterPtr-1), *(letterPtr), '\0'};
                vfprintf(stream, additFlag2, argc);
            }
        }


        ++letterPtr;
    }
    va_end(argc);
    return SUCCESS;
}

void dumpsSignedInt(char* str, int *ptr, int* resPos) {
    // https://www.youtube.com/watch?v=zxb8DvLUqcM
    // little-endian
    unsigned char *bytePtr = (unsigned char *)ptr;

    for (int i = sizeof(int) - 1; i >= 0; --i) {
        for (int j = 7; j >= 0; --j) {
            *resPos += sprintf(str + *resPos, "%d", (bytePtr[i] >> j) & 1);
        }
        *resPos += sprintf(str + *resPos, " ");
    }
}

void dumpsUnsignedInt(char* str, unsigned int *ptr, int* resPos) {
    unsigned char *bytePtr = (unsigned char *)ptr;

    for (int i = sizeof(unsigned int) - 1; i >= 0; --i) {
        for (int j = 7; j >= 0; --j) {
            *resPos += sprintf(str + *resPos, "%d", (bytePtr[i] >> j) & 1);
        }
        *resPos += sprintf(str + *resPos, " ");
    }
}

void dumpsDouble(char* str, double *ptr, int* resPos) {
    unsigned char *bytePtr = (unsigned char *)ptr;

    for (int i = sizeof(double) - 1; i >= 0; --i) {
        for (int j = 7; j >= 0; --j) {
            *resPos += sprintf(str + *resPos, "%d", (bytePtr[i] >> j) & 1);
        }
        *resPos += sprintf(str + *resPos, " ");
    }
}

void dumpsFloat(char* str, float *ptr, int* resPos) {
    unsigned char *bytePtr = (unsigned char *)ptr;

    for (int i = sizeof(float) - 1; i >= 0; --i) {
        for (int j = 7; j >= 0; --j) {
            *resPos += sprintf(str + *resPos, "%d", (bytePtr[i] >> j) & 1);
        }
        *resPos += sprintf(str + *resPos, " ");
    }
}

//resPos += sprintf(str + resPos, 

ErrorCode oversprintf(char* str, const char *format, ...) {
    if (format == NULL) return INCORRECT_INPUT;
    int resPos = 0;

    va_list argc;
    va_start(argc, format);

    const char* letterPtr = format;
    while (*letterPtr != '\0') {
        if (*letterPtr != '%') {
            resPos += sprintf(str + resPos,"%c", *letterPtr);
        } 
        else if (*(letterPtr+1) != '\0' && *(letterPtr+2) != '\0')
        {
            char additFlag[] = {*(letterPtr+1), *(letterPtr+2), '\0'};
            letterPtr += 2;
            const int resLen = 50;
            if (strcmp(additFlag, "Ro") == 0)
            {
                int num = va_arg(argc, int);
                char result[resLen];
                for (int i = 0; i < resLen - 1; ++i) result[i] = 'x';
                result[sizeof(result) - 1] = '\0';

                switch (intToRoman(num, result, resLen)) {
                    default:
                        break;
                    case INCORRECT_INPUT:
                        va_end(argc);
                        return INCORRECT_INPUT;
                }
                resPos += sprintf(str + resPos, "%s", result);
            }
            else if (strcmp(additFlag, "Zr") == 0)
            {
                unsigned int num = va_arg(argc, unsigned int);
                char result[resLen];
                for (int i = 0; i < resLen - 1; ++i) result[i] = 'x';
                result[sizeof(result) - 1] = '\0';

                switch (cekendorfRepresentation(num, result, resLen)) {
                    default:
                        break;
                    case OVERFLOW_ERROR:
                        va_end(argc);
                        return OVERFLOW_ERROR;
                    case INCORRECT_INPUT:
                        va_end(argc);
                        return INCORRECT_INPUT;
                }
                resPos += sprintf(str + resPos, "%s", result);
            }
            else if (strcmp(additFlag, "Cv") == 0 || strcmp(additFlag, "CV") == 0)
            {
                int num = va_arg(argc, int);
                int base = va_arg(argc, int);
                char* result;

                switch (fromDecimal(num, base, isupper(additFlag[1]), &result)) {
                    default:
                        break;
                    case MALLOC_ERROR:
                        va_end(argc);
                        return MALLOC_ERROR;
                }
                resPos += sprintf(str + resPos, "%s", result);
                free(result);
            }
            else if (strcmp(additFlag, "to") == 0 || strcmp(additFlag, "TO") == 0)
            {
                const char* num = va_arg(argc, const char*);
                int base = va_arg(argc, int);
                int res;

                switch (toDecimal(num, base, &res)) {
                    default:
                        break;
                    case MALLOC_ERROR:
                        va_end(argc);
                        return MALLOC_ERROR;
                }
                resPos += sprintf(str + resPos,"%d", res);
            }
            else if (strcmp(additFlag, "mi") == 0)
            {
                int num = va_arg(argc, int);
                dumpsSignedInt(str, &num, &resPos);
            }
            else if (strcmp(additFlag, "mu") == 0)
            {
                unsigned int num = va_arg(argc, unsigned int);
                dumpsUnsignedInt(str, &num, &resPos);
            }
            else if (strcmp(additFlag, "md") == 0)
            {
                double num = va_arg(argc, double);
                dumpsDouble(str, &num, &resPos);
            }
            else if (strcmp(additFlag, "mf") == 0)
            {
                float num = va_arg(argc, double); 
                // float num = va_arg(argc, float);
                dumpsFloat(str, &num, &resPos);
            }
            else
            {
                char additFlag2[] = {'%', *(letterPtr-1), *(letterPtr), '\0'};
                resPos += vsprintf(str + resPos, additFlag2, argc);
            }
        }


        ++letterPtr;
    }
    va_end(argc);
    return SUCCESS;
}