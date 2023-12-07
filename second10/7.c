#include <stdio.h>
#include <math.h>

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

float f1(float x) {
    // y = x
    return x;
}

float f2(float x) {
    // y = x^2
    return x * x + 1 - x; // корня нет
}

float f3(float x) {
    // y = -x
    return -x + 5;
}

float f4(float x) {
    // y = x^2 + x + 0.25
    return x*x + x + 0.25;
}

float f5(float x) {
    // y = x^2 - x + 0.24
    return x*x - x + 0.24;
}

float findRoot(float a, float b, float epsilon, float (*foo)(float)) {
    // ищем место, где функция меняет знак
    if (epsilon < 0)
        epsilon *= -1;
    if (!foo)
        return NAN;
    if (a > b) {
        float temp = a;
        a = b;
        b = temp;
    }
    if (foo(a) * foo(b) > 0) {
        // на промежутке нет корня
        return NAN;
    }

    
    while (b - a > epsilon) {
        float c = (a + b) / 2;

        if (foo(c) == 0)
            return c;
        else if (foo(c) * foo(a) < 0)
            b = c;
        else
            a = c;
    }
    return (a + b) / 2;
}

int main() {
    float a = -100;
    float b = 100;
    float epsilon = 0.000001;
    printf("0 = x: %f\n", findRoot(a, b, epsilon, f1));
    printf("0 = -x+5: %f\n", findRoot(a, b, epsilon, f3));
    printf("0 = x^2-x+1: %f\n", findRoot(a, b, epsilon, f2));
    printf("0 = x*x - x + 0.24: %f\n", findRoot(0, 0.5, epsilon, f5));
    fflush(stdout);
    
    
    return SUCCESS;
}