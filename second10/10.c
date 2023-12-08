#include <stdarg.h>
#include <stdio.h>
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

void d(const int rank, double fArrray[]) {
    if (!fArrray)
        return;
    // 1 + 2x + 3x^2 -> 2*1, 3*2, ...
    for (int i = 0; i < rank; ++i)
        fArrray[i] = fArrray[i+1] * (i+1);
}

ErrorCode f(const int rank, double x, const double fArrray[], double *res) {
    if (!res || !fArrray || rank < 0)
        return INCORRECT_INPUT;
    *res = 0;
    int curX = 1;
    for (int i = 0; i <= rank; ++i) {
        *res += fArrray[i] * curX;
        curX *= x;
    }
    return SUCCESS;
}

ErrorCode findGArray(const double epsilon, const double a, double **res, int rank, ...) {
    if (!res || rank < 0)
        return INCORRECT_INPUT;

    *res = (double*)malloc((rank+1) * sizeof(double));
    if (*res == NULL)
        return MALLOC_ERROR;

    double *fArrray = (double*)malloc((rank+1) * sizeof(double));
    if (*res == NULL) {
        free(res);
        return MALLOC_ERROR;
    }
    va_list fList;
    va_start(fList, rank);
    for (int i = 0; i <= rank; ++i)
        fArrray[i] = va_arg(fList, double);
    va_end(fList);

    double factN = 1; // факториал для знаменателя формулы тейлоа -> n!
    for (int i = 0; i <= rank; ++i) {
        double g;
        // g_0 = f(a)
        // g_1 = f(a)'/1!
        // g_2 = f(a)''/2!
        (void)f(rank-i, a, fArrray, &g);
        d(rank, fArrray);

        if (i > 0) {
            factN *= i;
            g /= factN;
        }
        (*res)[i] = g;
    }
    free(fArrray);
    return SUCCESS;
}

int main() {
    // f(x) = f₀ + f₁x¹ + ... + fₙxⁿ = g₀ + g₁(x-a)¹ + ... + gₙ(x-a)ⁿ

    // f(x) = 2 + 3x + 4x², a = 2

    double *res;
    int rank = 2;
    findGArray(0, 2, &res, rank, 2.0, 3.0, 4.0);
    printf("g₀₋ₙ: ");
    for (int i = 0; i <= rank; ++i)
        printf("%lf ", res[i]);
    fflush(stdout);

    // 24 + 19(x-2) + 4(x-2)² = 24-38+19x+4(x²-4x+4) = -14+19x+4x²-16x+16 = 4x²+3x+2

    free(res);
    return SUCCESS;
}