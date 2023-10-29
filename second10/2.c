#include <asm-generic/errno-base.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define DEBUG 0

#ifdef DEBUG
#define DEBUG_PRINT(format, ...) printf("Debug: "); printf(format, ##__VA_ARGS__); fflush(stdout)
#else
#define DEBUG_PRINT(msg)
#endif

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

ErrorCode getInput(char* buffer, int BUFFER_SIZE);
double myPow(double base, double exponent);
double geometricMean(int count, ...);
double power(double base, int degree, int* exitStatus);

int main(int argc, char *argv[]) {
    // 1 часть
    int exitStatus = UNKNOWN_ERROR;
    double result = geometricMean(4, 1.0, 2.0, 3.0, 4.0);
    printf("Среднее геометрическое: %lf\n", result);
    // 2 часть
    printf("Введите основание: ");

    const int BUFFER_SIZE = 50;
    char buffer[BUFFER_SIZE];
    switch (getInput(buffer, BUFFER_SIZE)) {
        case INCORRECT_INPUT:
            printf("%s\n", errorMessages[INCORRECT_INPUT]);
            return INCORRECT_INPUT;
        default:
            break;
    }

    char* input = strtok(buffer, "\n");
    if (input == NULL) {
        printf("%s\n", errorMessages[INCORRECT_INPUT]);
        return INCORRECT_INPUT;
    }
    
    char *endptr;
    double base = strtod(input, &endptr);
    if (*endptr != '\0' && *endptr != '\n') {
        printf("%s\n", errorMessages[INCORRECT_INPUT]);
        return INCORRECT_INPUT;
    }

    printf("Введите степень: ");
    switch (getInput(buffer, BUFFER_SIZE)) {
        case INCORRECT_INPUT:
            printf("%s\n", errorMessages[INCORRECT_INPUT]);
            return INCORRECT_INPUT;
        default:
            break;
    }

    input = strtok(buffer, "\n");
    if (input == NULL) {
        printf("%s\n", errorMessages[INCORRECT_INPUT]);
        return INCORRECT_INPUT;
    }

    //int degree = atoi(input); // atoi не обрабатывает ошибки
    int degree = strtol(input, &endptr, 10);
    if (*endptr != '\0' && *endptr != '\n') {
        printf("%s\n", errorMessages[INCORRECT_INPUT]);
        return INCORRECT_INPUT;
    }

    exitStatus = UNKNOWN_ERROR;
    result = power(base, degree, &exitStatus);
    switch (exitStatus) {
        default:
            break;
        case UNKNOWN_ERROR:
            printf("%s\n", errorMessages[UNKNOWN_ERROR]);
            return UNKNOWN_ERROR;
    }
    if (degree < 0) {
        result = 1 / result;
    }

    printf("%lf в степени %d равно %lf\n", base, degree, result);
    
    printf("%s\n", errorMessages[SUCCESS]);
    return SUCCESS;
}

ErrorCode getInput(char* buffer, int BUFFER_SIZE) 
{
    //const int BUFFER_SIZE = 50;
    //char buffer[BUFFER_SIZE];
    for (int i = 0; i < BUFFER_SIZE; ++i) buffer[i] = '\0';
    if (!fgets(buffer, sizeof(char) * BUFFER_SIZE, stdin)) 
    {
        //printf("%s\n", errorMessages[INCORRECT_INPUT]);
        return INCORRECT_INPUT;
    }
    if (!(buffer[BUFFER_SIZE - 1] == '\0' || buffer[BUFFER_SIZE - 2] == '\n'))
    {
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }
    // char* input = strtok(buffer, "\n");
    // if (input == NULL) 
    // {
    //     return INCORRECT_INPUT;
    // }
    return SUCCESS;
}

double geometricMean(int count, ...) {
    if (count <= 0) {
        return 0.0;
    }
    va_list args;
    va_start(args, count);

    double product = 1.0;

    for (int i = 0; i < count; ++i) {
        double value = va_arg(args, double);
        product *= value;
    }

    va_end(args);
    return pow(product, 1.0 / count);
}

double power(double base, int degree, int* exitStatus) {
    // a^8 = (a^4)^2 = (((a)^2)^2)^2
    degree = abs(degree);
    if (degree == 0) {
        *exitStatus = SUCCESS;
        return 1.0;
    }

    double temp = power(base, degree / 2, exitStatus);
    DEBUG_PRINT("%d -> %lf\n", degree / 2, temp);
    if (degree % 2 == 0) {
        // степень чётная
        *exitStatus = SUCCESS;
        return temp * temp;
    } else {
        // степень нечётная
        *exitStatus = SUCCESS;
        return base * temp * temp;
    }
}