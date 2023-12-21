#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>

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

#define BUFFER_SIZE 100
#define FLAG_SIZE 20
#define INPUT_NUM_MAXLEN 100

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

// преобразование цекендорфова представления в int
unsigned int cekendorfToUInt(const char *cekendorf) {
    int length = strlen(cekendorf);
    unsigned int result = 0;
    unsigned int fib1 = 1, fib2 = 1;

    for (int i = 0; i < length; ++i) {
        if (cekendorf[i] == '1') {
            result += fib1;
        }

        unsigned int temp = fib1;
        fib1 = fib2;
        fib2 += temp;
    }

    return result - (fib2 - fib1);
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
        *res = decimal;
        return OVERFLOW_ERROR;
    }

    if (*endPtr != '\0') {
        //printf("Ошибка при преобразовании строки в число, грустно 😿\n");
        *res = decimal;
        return INCORRECT_INPUT;
    }

    *res = decimal;
    return SUCCESS;
}

int processUserFlags(FILE* stream, const char* format, va_list* ptr, char * str)
{
    if (!stream && !str)
        return -1;
    int varsRead = 0;
    while (*format != '\0')
    {
        if (*format == '%' && *(format + 1) == 'R' && *(format + 2) == 'o')
        {
            format += 3;
            int* num = va_arg(*ptr, int*);
            char romanStr[INPUT_NUM_MAXLEN];
            if (stream) {
                if (fscanf(stream, "%s", romanStr) != 1) 
                    return -1;
            } else {
                int numCharsRead;
                if (sscanf(str, "%s %n", romanStr, &numCharsRead) != 1) return -1;
                str += numCharsRead; // move to end
            }

            *num = romanToInt(romanStr);
        }
        else if (*format == '%' && *(format + 1) == 'Z' && *(format + 2) == 'r')
        {
            format += 3;
            unsigned int* num = va_arg(*ptr, unsigned int*);
            char cyckendorfStr[INPUT_NUM_MAXLEN];
            // %s - до пробела или конца буфера
            if (stream) {
                if (fscanf(stream, "%s", cyckendorfStr) != 1)
                    return -1;
            } else {
                int numCharsRead;
                if (sscanf(str, "%s %n", cyckendorfStr, &numCharsRead) != 1) return -1;
                str += numCharsRead;
            }

            // cekendorfRepresentation(unsigned int n, char *result, int resLen)
            *num = cekendorfToUInt(cyckendorfStr);
        }
        else if (*format == '%' && *(format + 1) == 'C' && (*(format + 2) == 'v' || *(format + 2) == 'V'))
        {
            char flag = *(format + 2);
            format += 3;
            int* num = va_arg(*ptr, int*);
            int base = va_arg(*ptr, int);
            char numStr[INPUT_NUM_MAXLEN];
            
            if (stream) {
                if (fscanf(stream, "%s", numStr) != 1) 
                    return -1;
            } else {
                int numCharsRead;
                if (sscanf(str, "%s %n", numStr, &numCharsRead) != 1) return -1;
                str += numCharsRead;
            }

            for(int i = 0; i < INPUT_NUM_MAXLEN; ++i) {
                if (islower(numStr[i]) && flag == 'V') numStr[i] = '\0';
                if (isupper(numStr[i]) && flag == 'v') numStr[i] = '\0';
            }
            ErrorCode code = toDecimal(numStr, base, num);
            if (code == SUCCESS) ++varsRead;
        }
        else if (*format == '%') 
        {
            char flag[FLAG_SIZE];
            int i = 0;
            flag[i++] = *format;
            ++format;
            while (*format != ' ' && *format != '\t' && *format != '\n' && *format != '%' && *format != 'd' && *format != 'f' && *format != 'n' && *format != 'c' && *format != 's')
            {
                if (i < FLAG_SIZE - 1)
                    flag[i++] = *format;
                ++format;
            }
            if (i < FLAG_SIZE - 1)
                flag[i++] = *format;
            flag[i] = '\0';
            ++format;
            if (flag[i - 1] == 'c')
            {
                char c;
                while ((stream != NULL && (c = fgetc(stream)) == ' ') || c == '\t' || c == '\n'){}
                if (c != EOF && stream != NULL) *(va_arg(*ptr, char*)) = c;
                while ((stream == NULL && (c = sscanf(str, "%c", &c)) == ' ') || c == '\t' || c == '\n'){str++;}
                if (c != '\0' && stream == NULL) *(va_arg(*ptr, char*)) = c;
            }
            else
            {
                if (stream) 
                    vfscanf(stream, flag, *ptr);
                else 
                {
                    int numCharsRead = 0;
                    char buffer[BUFFER_SIZE];
                    if (sscanf(str, "%s %n", buffer, &numCharsRead) != 1) return -1;
                    vsscanf(buffer, flag, *ptr);
                    str += numCharsRead;
                }
            }
            ++varsRead;
        }
        else
            ++format;
    }
    return varsRead;
}

int overfscanf(FILE* stream, const char* format, ...)
{
    va_list ptr;
    va_start(ptr, format);

    int varsRead = processUserFlags(stream, format, &ptr, NULL);
    va_end(ptr);
    return varsRead;
}

int oversscanf(char* str, const char* format, ...)
{
    va_list ptr;
    va_start(ptr, format);

    int varsRead = processUserFlags(NULL, format, &ptr, str);
    va_end(ptr);
    return varsRead;
}

int main() {
    int a = 100;
    char str[10] = "IV";
    oversscanf(str, " %Ro", &a);
    printf("%d\n", a);
    fflush(stdout);
    unsigned int b = 100;
    overfscanf(stdin, "%Zr", &b); // 11
    printf("%u\n", b);
    fflush(stdout);
    overfscanf(stdin, "%CV", &a, 36); // Z
    printf("%d\n", a);
    fflush(stdout);
    return SUCCESS;
}