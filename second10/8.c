#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

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

#include <stdio.h>
#include <string.h>

int charToInt(char c) {
    if (c >= '0' && c <= '9')
        return c - '0';
    else if (islower(c))
        return c - 'a' + 10;
    else
        return c - 'A' + 10;
}

char intToChar(int n) {
    if (n >= 0 && n <= 9) 
        return '0' + n;
    else 
        return 'A' + n - 10;
}

ErrorCode addInBase(const char num1[], const char num2[], const int base, char *res[]) {
    if (!num1 || !num2 || !res || base < 2 || base > 36)
        return INCORRECT_INPUT;

    while (num1[0] == '0')
        ++num1;
    while (num2[0] == '0')
        ++num2;
    int len1 = strlen(num1);
    int len2 = strlen(num2);
    int maxLen = len1 > len2 ? len1 : len2;
    // максимум прибавится один разряд за сложение
    char* result = (char*)malloc((maxLen + 2) * sizeof(char));
    if (!result)
        return MALLOC_ERROR;

    int carry = 0;
    for (int i = 0; i <= maxLen; ++i) {
        int digit1 = (i < len1) ? charToInt(num1[len1-1-i]) : 0;
        int digit2 = (i < len2) ? charToInt(num2[len2-1-i]) : 0;
        int sum = digit1 + digit2 + carry;
        carry = sum / base;
        result[maxLen - i] = intToChar(sum % base);
    }
    result[maxLen+1] = '\0';

    if (result[0] == '0') {
        char *temp = (char*)malloc((maxLen + 1) * sizeof(char));
        if (!temp) {
            free(result);
            return MALLOC_ERROR;
        }
        for (int i = 0; i <= maxLen; ++i)
            temp[i] = result[i+1];
        free(result);
        *res = temp;
        return SUCCESS;
    }
    *res = result;
    return SUCCESS;
}

ErrorCode validateNumber(char num[]) {
    if (!num)
        return INCORRECT_INPUT;
    int len = strlen(num);
    for (int i = 0; i < len; ++i) {
        if (!isdigit(num[i]) && !isalpha(num[i]))
            return INCORRECT_INPUT;
    }
    return SUCCESS;
}

ErrorCode numbersAddition(int base, int parCnt, ...) {
    if (base < 2 || base > 36 || parCnt < 1) 
        return INCORRECT_INPUT;

    char *summ = (char*)malloc(2 * sizeof(char));
    if (!summ)
        return MALLOC_ERROR;
    summ[0] = '0';
    summ[1] = '\0';
    
    va_list nums;
    va_start(nums, parCnt);
    for (int i = 0; i < parCnt; ++i) {
        char *num = va_arg(nums, char*);
        if (validateNumber(num) != SUCCESS) {
            printf("Число %s не прошло проверку\n", num);
            fflush(stdout);
            continue;
        } 

        char *temp;
        ErrorCode code = addInBase(summ, num, base, &temp);
        switch (code) {
            case SUCCESS:
                free(summ);
                summ = temp;
                break;
            default:
                // пропускаем число тогда
                printf("%s\n", errorMessages[code]);
                fflush(stdout);
                break;
        }
    }
    va_end(nums);
    printf("Результат суммы: %s\n", summ);
    fflush(stdout);
    free(summ);
    return SUCCESS;
}

int main() {
    // char num1[] = "9123";
    // char num2[] = "0456";
    // int base = 10;
    // char* result;
    // addInBase(num1, num2, base, &result);
    // printf("Сумма: %s\n", result);
    // free(result);

    numbersAddition(16, 6, "10", "01", "!!!", "1", "00000005", "5");
    numbersAddition(10, 4, "9876543210", "9876543210", "9876543210", "9876543210");
    printf("%ld\n", 9876543210 * 4);
    numbersAddition(36, 4, "z", "1", "A", "1");


    return 0;
}