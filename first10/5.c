#include <ctype.h>
#include <math.h>
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

#define MAX_ITERATIONS 100
#define BUFFER_SIZE 100

ErrorCode factorial(int n, unsigned long long *res) {
    if (n < 0 || !res) 
        return INCORRECT_INPUT;
    if (n == 0 || n == 1) {
        *res = 1;
        return SUCCESS;
    }

    *res = 1;
    for (int i = 2; i <= n; ++i) {
        if (*res * i < *res)
            return OVERFLOW_ERROR;
        *res *= i;
    }
    return SUCCESS;
}

ErrorCode factoriall(int n, unsigned long long *res) {
    if (n < 0 || !res) 
        return INCORRECT_INPUT;
    if (n == 0 || n == 1) {
        *res = 1;
        return SUCCESS;
    }

    *res = 1;
    if (n % 2 == 0) {
        for (int i = 2; i <= n; i+=2) {
            if (*res * i < *res)
                return OVERFLOW_ERROR;
            *res *= i;
        }
    }
    else {
        for (int i = 3; i <= n; i+=2) {
            if (*res * i < *res)
                return OVERFLOW_ERROR;
            *res *= i;
        }
    }

    return SUCCESS;
}

ErrorCode summA(double epsilon, double x, double *res) {
    if (!res)
        return INCORRECT_INPUT;
    if (epsilon < 0)
        epsilon *= -1;

    // int n = 0;
    double summ1 = 0;
    double summ2 = 0;
    unsigned long long factN = 1;
    double curX = 1;
    for (int n = 0; n < MAX_ITERATIONS; ++n) {
        // для 10 + 5 -> х1.5 запаса нужно
        // для 5 + 5 -> х2 запаса нужно
        double ratio = 1 + (curX/factN) / summ1;
        if (summ1*ratio/ratio - summ1 > 10.0) {
            *res = summ1;
            return OVERFLOW_ERROR;
        }
        summ1 += curX/factN;

        if (fabs(summ1 - summ2) < epsilon) {
            *res = summ1;
            return SUCCESS;
        }
        summ2 = summ1;

        // if (fabs(curX*x) < fabs(curX)) {
        //     *res = summ1;
        //     return OVERFLOW_ERROR;
        // }
        if (curX*x/x - curX > 10.0) {
            *res = summ1;
            return OVERFLOW_ERROR;
        }
        curX *= x;

        if (factN*(n+1) < factN) { // ok
            *res = summ1;
            return OVERFLOW_ERROR;
        }
        factN *= n+1;

    }
    *res = summ1;
    if (fabs(summ1) - fabs(summ2) < epsilon)
        return SUCCESS;
    else
        return OVERFLOW_ERROR;
}

ErrorCode summB(double epsilon, double x, double *res) {
    if (!res)
        return INCORRECT_INPUT;
    if (epsilon < 0)
        epsilon *= -1;

    double summ1 = 0;
    double summ2 = 0;
    unsigned long long factN = 1;
    double curX = 1;
    // [-1^n * x^2n] / (2n)!
    for (int n = 0; n < MAX_ITERATIONS; ++n) {
        double ratio = 1 + (curX/factN) / summ1;
        if (summ1*ratio/ratio - summ1 > 10.0) {
            *res = summ1;
            return OVERFLOW_ERROR;
        }
        summ1 += curX/factN;

        if (fabs(summ1 - summ2) < epsilon) {
            *res = summ1;
            return SUCCESS;
        }
        summ2 = summ1;

        if (curX*x/x - curX > 10.0) {
            *res = summ1;
            return OVERFLOW_ERROR;
        }
        curX = pow(x, 2*(n+1));
        // curX *= pow(-1, (n+1));
        if ((n+1) % 2 != 0) {
            curX *= -1;
        }

        ErrorCode code = factorial(2*(n+1), &factN);
        if (code != SUCCESS) {
            *res = summ1;
            return code;
        }
    }
    *res = summ1;
    if (fabs(summ1) - fabs(summ2) < epsilon)
        return SUCCESS;
    else
        return OVERFLOW_ERROR;
}

ErrorCode summC(double epsilon, double x, double *res) {
    if (!res)
        return INCORRECT_INPUT;
    if (epsilon < 0)
        epsilon *= -1;

    double summ1 = 0;
    double summ2 = 0;
    unsigned long long factN = 1;
    double curX = 1;
    // [3^3n * (n!)^3 x^2n] / (3n)!
    for (int n = 0; n < MAX_ITERATIONS; ++n) {
        double ratio = 1 + (curX/factN) / summ1;
        if (summ1*ratio/ratio - summ1 > 10.0) {
            *res = summ1;
            return OVERFLOW_ERROR;
        }
        summ1 += curX/factN;

        if (fabs(summ1 - summ2) < epsilon) {
            *res = summ1;
            return SUCCESS;
        }
        summ2 = summ1;

        if (curX*x/x - curX > 10.0) {
            *res = summ1;
            return OVERFLOW_ERROR;
        }
        curX = pow(x, 2*(n+1));
        curX *= pow(3, 3*(n+1));
        ErrorCode code = factorial((n+1), &factN);
        if (code != SUCCESS) {
            *res = summ1;
            return code;
        }
        curX *= pow(factN, 3);
        
        code = factorial(3*(n+1), &factN);
        if (code != SUCCESS) {
            *res = summ1;
            return code;
        }
    }
    *res = summ1;
    if (fabs(summ1) - fabs(summ2) < epsilon)
        return SUCCESS;
    else
        return OVERFLOW_ERROR;
}

ErrorCode summD(double epsilon, double x, double *res) {
    if (!res)
        return INCORRECT_INPUT;
    if (epsilon < 0)
        epsilon *= -1;

    double summ1 = 0;
    double summ2 = 0;
    unsigned long long factN = 1;
    double curX = 1;
    // [-1^n * (2n-1)!! * x^2n] / (2n)!!
    for (int n = 0; n < MAX_ITERATIONS; ++n) { // n = 1
        if (n != 0) {
            double ratio = 1 + (curX/factN) / summ1;
            if (summ1*ratio/ratio - summ1 > 10.0) {
                *res = summ1;
                return OVERFLOW_ERROR;
            }
            summ1 += curX/factN;

            if (fabs(summ1 - summ2) < epsilon) {
                *res = summ1;
                return SUCCESS;
            }
            summ2 = summ1;
        }

        if (curX*x/x - curX > 10.0) {
            *res = summ1;
            return OVERFLOW_ERROR;
        }
        curX = pow(x, 2*(n+1));
        if ((n+1) % 2 != 0) {
            curX *= -1;
        }
        ErrorCode code = factoriall(2*n + 1, &factN);
        if (code != SUCCESS) {
            *res = summ1;
            return code;
        }
        curX *= factN;
        
        code = factoriall(2*(n+1), &factN);
        if (code != SUCCESS) {
            *res = summ1;
            return code;
        }
    }
    *res = summ1;
    if (fabs(summ1) - fabs(summ2) < epsilon)
        return SUCCESS;
    else
        return OVERFLOW_ERROR;
}

void printHelp() {
    printf("Вычисление значения рядов с точностью ε, где ε (вещественное число) подаётся "\
    "программе в виде аргумента командной строки:\nпрограмма.exe -e число");
}

int main(int argc, char *argv[]) {
    if (argc > 1 && strcmp(argv[1], "-h") == 0) {
        printHelp();
        return 0;
    } else if (argc == 3 && strcmp(argv[1], "-e") == 0) {
        //double epsilon = atof(argv[2]);
        char *endptr;
        double epsilon = strtod(argv[2], &endptr);
        if (*endptr != '\0') {
            printf("%s\n", errorMessages[INCORRECT_INPUT]);
            fflush(stdout);
            return INCORRECT_INPUT;
        }
        printf("ε = %.16lf\n", epsilon);

        printf("Введите x: ");
        fflush(stdout);
        char input[BUFFER_SIZE];
        fgets(input, BUFFER_SIZE, stdin);

        char *inpPtr = input;
        while (isspace(*inpPtr)) {
            if (*inpPtr == '\n') {
                printf("%s\n", errorMessages[INCORRECT_INPUT]);
                fflush(stdout);
                return INCORRECT_INPUT;
            }
            ++inpPtr;
        }

        double x = strtod(input, &endptr);
        if (*endptr != '\n') {
            printf("%s\n", errorMessages[INCORRECT_INPUT]);
            fflush(stdout);
            return INCORRECT_INPUT;
        }
        printf("x = %lf\n", x);

        // ErrorCode (*summNum)(double, double, double*);
        for (int fNumber = 1; fNumber < 5; ++fNumber) {
            double Result;
            ErrorCode code;
            switch (fNumber) {
                case 1:
                    code = summA(epsilon, x, &Result);
                    break;
                case 2:
                    code = summB(epsilon, x, &Result);
                    break;
                case 3:
                    code = summC(epsilon, x, &Result);
                    break;
                case 4:
                    code = summD(epsilon, x, &Result);
                    break;
            }

            printf("Сумма #%d = %.16lf\n", fNumber, Result);
            if (code != SUCCESS)
                printf(" -> %s\n", errorMessages[code]);
        }
        printf("На этом всё, хорошего дня 👋");
        return 0;
    } else {
        printHelp();
        return 0;
    }

    return 0;
}
