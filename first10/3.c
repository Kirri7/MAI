#include <math.h>
#include <stdio.h>
#include <string.h>
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

int isRightAngledTriangle(float a, float b, float c, float epsilon) {
    // теорема Пифагора с эписилон
    float sides[3] = {a, b, c};
    float maxSide = 0, squareSum = 0;

    for (int i = 0; i < 3; ++i) {
        if (sides[i] > maxSide) {
            maxSide = sides[i];
        }
        squareSum += sides[i] * sides[i];
    }

    // a^2 + b^2 + c^2 = 2*c^2
    // a^2 + b^2 + c^2 - 2*c^2 = 0
    int answer = fabsf(squareSum - (2 * maxSide * maxSide)) <= epsilon;
    return answer;
}

void printQuadraticEquationRoots(float epsilon, float a, float b, float c) {
    float discriminant = b * b - 4 * a * c;

    if (fabsf(discriminant) <= epsilon) {
        float root = -b / (2 * a);
        printf("Один корень: x = %f\n", root);
    } else if (discriminant > epsilon) {
        float root1 = (-b + sqrtf(discriminant)) / (2 * a);
        float root2 = (-b - sqrtf(discriminant)) / (2 * a);
        printf("Два корня: x1 = %f, x2 = %f\n", root1, root2);
    } else {
        float realPart = -b / (2 * a);
        float imaginaryPart = sqrtf(fabsf(discriminant)) / (2 * a);
        printf("Два комплексных корня: x1 = %f + %fi, x2 = %f - %fi\n", realPart, imaginaryPart, realPart, imaginaryPart);
    }
}

void permutations(double epsilon, double a, double b, double c) {
    printQuadraticEquationRoots(epsilon, a, b, c);
    printQuadraticEquationRoots(epsilon, a, c, b);
    printQuadraticEquationRoots(epsilon, b, a, c);
    printQuadraticEquationRoots(epsilon, b, c, a);
    printQuadraticEquationRoots(epsilon, c, a, b);
    printQuadraticEquationRoots(epsilon, c, b, a);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("%s <флаг> числа...\n", argv[0]);
        return INCORRECT_INPUT;
    }

    char *flagArg = argv[1];
    if (strlen(flagArg) != 2) {
        printf("%s\n", errorMessages[INCORRECT_INPUT]);
        fflush(stdout);
        return INCORRECT_INPUT;
    }

    if (flagArg[0] != '-' && flagArg[0] != '/') {
        printf("%s\n", errorMessages[INCORRECT_INPUT]);
        fflush(stdout);
        return INCORRECT_INPUT;
    }
    
    if (flagArg[1] == 'q') 
    {
        if (argc != 6) {
            printf("... -q <epsilon> <a> <b> <c>\n");
            return INCORRECT_INPUT;
        }
        char* endptr;
        int epsilon = strtof(argv[2], &endptr);
        if (*endptr != '\0' && *endptr != '\n') 
        {
            printf("Некорректный ввод\n");
            fflush(stdout);
            return INCORRECT_INPUT;
        }
        int a = strtof(argv[3], &endptr);
        if (*endptr != '\0' && *endptr != '\n') 
        {
            printf("Некорректный ввод\n");
            fflush(stdout);
            return INCORRECT_INPUT;
        }
        int b = strtof(argv[4], &endptr);
        if (*endptr != '\0' && *endptr != '\n') 
        {
            printf("Некорректный ввод\n");
            fflush(stdout);
            return INCORRECT_INPUT;
        }
        int c = strtof(argv[5], &endptr);
        if (*endptr != '\0' && *endptr != '\n') 
        {
            printf("Некорректный ввод\n");
            fflush(stdout);
            return INCORRECT_INPUT;
        }

        permutations(epsilon, a, b, c);
    }
    else if (flagArg[1] == 'm') 
    {
        if (argc != 4) {
            printf("... -m <num1> <num2>\n");
            return INCORRECT_INPUT;
        }
        char* endptr;
        int a = strtol(argv[2], &endptr, 10);
        if (*endptr != '\0' && *endptr != '\n') 
        {
            printf("Некорректный ввод\n");
            fflush(stdout);
            return INCORRECT_INPUT;
        }
        int b = strtol(argv[3], &endptr, 10);
        if (*endptr != '\0' && *endptr != '\n') 
        {
            printf("Некорректный ввод\n");
            fflush(stdout);
            return INCORRECT_INPUT;
        }
        // кратно ли a числу b?
        if (a < 0) a *= -1;
        if (b < 0) b *= -1;
        if (a == 0)         {
            printf("На ноль делить нельзя\n");
            fflush(stdout);
            return INCORRECT_INPUT;
        }
        if (b % a == 0) {
            printf("a кратно b\n");
            fflush(stdout);
        } else {
            printf("a не кратно b\n");
            fflush(stdout);
        }
    }
    else if (flagArg[1] == 't') 
    {
        if (argc != 6) {
            printf("... -t <epsilon> <side1> <side2> <side3>\n");
            return INCORRECT_INPUT;
        }
        char* endptr;
        float epsilon = strtof(argv[2], &endptr);
        if (*endptr != '\0' && *endptr != '\n' || epsilon < 0) 
        {
            printf("Некорректный ввод\n");
            fflush(stdout);
            return INCORRECT_INPUT;
        }
        float a = strtof(argv[3], &endptr);
        if (*endptr != '\0' && *endptr != '\n' || a < 0) 
        {
            printf("Некорректный ввод\n");
            fflush(stdout);
            return INCORRECT_INPUT;
        }
        float b = strtof(argv[4], &endptr);
        if (*endptr != '\0' && *endptr != '\n' || b < 0) 
        {
            printf("Некорректный ввод\n");
            fflush(stdout);
            return INCORRECT_INPUT;
        }
        float c = strtof(argv[5], &endptr);
        if (*endptr != '\0' && *endptr != '\n' || c < 0) 
        {
            printf("Некорректный ввод\n");
            fflush(stdout);
            return INCORRECT_INPUT;
        }


        int answer = isRightAngledTriangle(a, b, c, epsilon);
        switch (answer) {
            case 1: 
                printf("Это прямоугольный треугольник\n");
                fflush(stdout);
                break;
            default:
                printf("Это не прямоугольный треугольник\n");
                fflush(stdout);\
                break;
        }   
    }
    else 
    {
        printf("Формат флага неверный\n");
        fflush(stdout);
        return INCORRECT_INPUT;
    }
    
    return SUCCESS;
}