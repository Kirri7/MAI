#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#define DEBUG 0

#ifdef DEBUG
#define DEBUG_PRINT(format, ...) printf("Debug: "); printf(format, ##__VA_ARGS__); fflush(stdout)
#else
#define DEBUG_PRINT(msg)
#endif

#define printf(format, ...) printf(format, ##__VA_ARGS__); fflush(stdout);

//TODO сделать тесты для функций + скрины для защиты
//TODO сделать норм вывод

void printHelp();
double fx(int fNumber, double x);
double integrate(int fNumber, int n);

int main(int argc, char *argv[]) {
    
    if (argc > 1 && strcmp(argv[1], "-h") == 0) {
        printHelp();
        return 0;
    } else if (argc == 3 && strcmp(argv[1], "-e") == 0) {
        //double epsilon = atof(argv[2]);
        double epsilon = strtod(argv[2], NULL);
        printf("ε = %.16lf\n", epsilon);

        for (int fNumber = 1; fNumber < 5; ++fNumber) {
            double preResult = integrate(fNumber, 1);
            double curResult = integrate(fNumber, 2);
            int n = 3;

            while (fabs(preResult - curResult) > epsilon) {
                preResult = curResult;
                curResult = integrate(fNumber, n++);
                if (n > 10000) { // Верхняя граница делений отрезка
                    printf(" ... слишком долго ждём, хватит вычислений:\n")
                    break;
                }
            }

            printf("Интеграл #%d = %.16lf\n", fNumber, curResult);
        }
        printf("На этом всё, хорошего дня 👋");
        return 0;
    } else {
        printHelp();
        return 0;
    }

    return 0;
}

void printHelp() {
    printf("Вычисление значения интегралов с точностью ε, где ε (вещественное число) подаётся "\
    "программе в виде аргумента командной строки:\nпрограмма.exe -e число");
}

double fx(int fNumber, double x) {
    if (fNumber == 1) // y = ln(1+x)/x
        return log(1 + x) / x;
    else if (fNumber == 2) // y = e^({-x^2}/2)
        return exp(-pow(x, 2) / 2);
    else if (fNumber == 3) // y = ln(1/{1-x})
        return log(1 / (1 - x));
    else if (fNumber == 4) // y = x^x
        return pow(x, x);
    else // y = ?
        return 0; 
}

double integrate(int fNumber, int n) {
    double dx = (double)(1 - 0) / n; // приращение к x
    double result = 0;

    for (int i = 1; i <= n; ++i) {
        double x_i = i * dx; // правая точка i-отрезка
        result += dx * fx(fNumber, x_i); // площадь i-прямоугольника
    }

    return result;
}