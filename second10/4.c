#include <math.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
// #include "2.c"

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

typedef struct {
    double x, y;
} Dot;

double findParallelogramArea(Dot a, Dot b, Dot c) {
    // пусть a - начало координат
    double x1 = b.x - a.x;
    double y1 = b.y - a.y;
    double x2 = c.x - a.x;
    double y2 = c.y - a.y;
    // определитель координат векторов = S
    // +S -> i j
    // -S -> j i
    return x1 * y2 - x2 * y1;
}

ErrorCode isConvex(int* res, int dotCnt, ...){
    // порядок передачи точек задаёт последовательность граней
    if (dotCnt < 3 || !res)
        return INCORRECT_INPUT;

    va_list dots;
    va_start(dots, dotCnt);
    Dot* polygon = (Dot*)malloc(dotCnt * sizeof(Dot));

    for(int i = 0; i < dotCnt; ++i)
        polygon[i] = va_arg(dots, Dot);
    
    va_end(dots);

    // for(int i = 0; i < dotCnt; ++i)
    //     printf("%lf, %lf\n", polygon[i].x,  polygon[i].y);

    // все площади либо положительные, либо отрицательные должны быть
    double S = findParallelogramArea(polygon[0], polygon[1], polygon[2]);
    int areaSign = 1;
    if (S < 0) 
        areaSign = -1;
    
    for(int i = 0; i < dotCnt; ++i) {
        Dot prev = polygon[(i-1+dotCnt)%dotCnt];
        Dot curr = polygon[i];
        Dot next = polygon[(i+1)%dotCnt];

        S = findParallelogramArea(prev, curr, next);
        if (S == 0) {
            // грани совпадают
            free(polygon);
            return INCORRECT_INPUT;
        } 
        else if (S > 0 && areaSign != 1 || S < 0 && areaSign != -1) {
            // найден вогнутый участок
            *res = 0;
            free(polygon);
            return SUCCESS;
        }
    }

    *res = 1;
    free(polygon);
    return SUCCESS;
}

double power(double base, int degree) {
    // a^8 = (a^4)^2 = (((a)^2)^2)^2
    degree = abs(degree);
    if (degree == 0) {
        return 1.0;
    }

    double temp = power(base, degree / 2);
    // DEBUG_PRINT("%d -> %lf\n", degree / 2, temp);
    if (degree % 2 == 0) {
        // степень чётная
        return temp * temp;
    } else {
        // степень нечётная
        return base * temp * temp;
    }
}

ErrorCode findPolyValue(double* res, double x, int degree, ...){
    if (!res || degree < 0)
        return INCORRECT_INPUT;
    *res = 0;

    va_list coefs;
    va_start(coefs, degree);

    double curX = power(x, degree);

    for (int i = degree; i > 0; --i){
        double coef = va_arg(coefs, double);
        *res += coef * curX;
        if (x != 0)
            curX /= x;
    }
    double coef = va_arg(coefs, double);
    *res += coef; // x^0

    va_end(coefs);
    return SUCCESS;
}

int main() {
    Dot d00 = {0, 0};
    Dot d10 = {1, 0};
    Dot d11 = {1, 1};
    Dot roof = {0.5, 1.5};
    Dot crack = {0.5, 0.5};
    Dot d01 = {0, 1};

    int res = 100;
    isConvex(&res, 5, d00, d10, d11, roof, d01);
    printf("Выпуклый: %d\n", res);

    res = 100;
    isConvex(&res, 5, d00, d10, d11, crack, d01);
    printf("Выпуклый: %d\n", res);
    fflush(stdout);
    
    double ress = 100;
    findPolyValue(&ress, 0.0, 2, 1.0, 1.0, 1.0);
    printf("Полином: %lf\n", ress);
    fflush(stdout);

    ress = 100;
    // 2^4 + 2^3 + 2^2 + 2 = 16+8+4+2 = 30
    findPolyValue(&ress, 1.9999, 3, 2.01, 2.0, 1.99, 2.0);
    printf("Полином: %lf\n", ress);
    fflush(stdout);
    
    return SUCCESS;
}