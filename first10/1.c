#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void printMultiples(int multiple) {
    if (multiple < 0) multiple *= -1;
    int found = 0;
    for (int i = 1; i <= 100; ++i) {
        if (i % multiple == 0) {
            printf("%d ", i);
            found = 1;
        }
    }

    if (!found) {
        printf("Кратных чисел нет");
    }

    printf("\n");
}

int isPrime(const int num) {
    if (num < 2) {
        return 0;
    }

    if (num == 2) return 1;

    if (num % 2 == 0) // делится на 2
        return 0;

    for (int i = 3; i*i <= num; i += 2) {
        if (num % i == 0) {
            return 0; // не простое
        }
    }

    return 1; // простое
}

void printPowerTable(int limit) {
    if (limit > 10) {
        return;
    }

    printf("Основание\tСтепень\tРезультат\n");
    for (int base = 1; base <= 10; ++base) {
        for (int degree = 1; degree <= limit; ++degree) {
            long long result = 1;
            for (int i = 0; i < degree; ++i) {
                result *= base;
            }
            printf("%d\t\t%d\t\t%lld\n", base, degree, result);
        }
    }
}

unsigned long long calculateSum(int limit) {
    unsigned long long sum = 0;
    for (int i = 1; i <= limit; ++i) {
        sum += i;
    }

    return sum;
}

unsigned long long factorial(int n) {
    if (n == 0 || n == 1) {
        return 1;
    } else {
        return n * factorial(n - 1);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("%s <число> <флаг>\n", argv[0]);
        return INCORRECT_INPUT;
    }

    char* endptr;
    int number = strtol(argv[1], &endptr, 10);
    if (*endptr != '\0' && *endptr != '\n') 
    {
        printf("Некорректный ввод\n");
        return INCORRECT_INPUT;
    }
    char *flagArg = argv[2];
    if (strlen(flagArg) != 2) {
        printf("%s\n", errorMessages[INCORRECT_INPUT]);
        fflush(stdout);
        return INCORRECT_INPUT;
    }

    if (flagArg[0] == '-' || flagArg[0] == '/') {
        char flag = flagArg[1];
        int numLen = strlen(argv[1]);
        switch (flag) {
            case 'h': // написать кратные заданному числа
                printMultiples(number);
                break;
            case 'p': // простое число?
                printf("%d\n", isPrime(number));
                fflush(stdout);
                break;
            case 's': // разделение на цифры с пробелом
                for (int i = 0; i < numLen; ++i) {
                    printf("%c ", argv[1][i]);
                    fflush(stdout);
                }
                printf("\n");
                fflush(stdout);
                break;
            case 'e': // таблица степеней 1..10 в степенях от 1 до числа
                if (number > 10 || number < 1) {
                    printf("%s\n", errorMessages[INCORRECT_INPUT]);
                    fflush(stdout);
                    return INCORRECT_INPUT;
                }
                printPowerTable(number);
                break;
            case 'a': // сумма от 1 до число
                if (number < 1) {
                    printf("%s\n", errorMessages[INCORRECT_INPUT]);
                    fflush(stdout);
                    return INCORRECT_INPUT;
                }
                printf("%llu\n", calculateSum(number));
                fflush(stdout);
                break;
            case 'f': // факториал 
                if (number < 0) {
                    printf("%s\n", errorMessages[INCORRECT_INPUT]);
                    fflush(stdout);
                    return INCORRECT_INPUT;
                }
                printf("%llu\n", factorial(number));
                fflush(stdout);
                break;
            default:
                printf("Такого флага нет: %c\n", flag);
                return INCORRECT_INPUT;
        }
    } else {
        printf("Формат флага неверный\n");
        fflush(stdout);
        return INCORRECT_INPUT;
    }

    printf("%s\n", errorMessages[SUCCESS]);
    fflush(stdout);
    return SUCCESS;
}