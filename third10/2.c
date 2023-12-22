#include <stdio.h>
#include <stdlib.h>
#include <time.h> //todo
#include <math.h>
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

typedef struct {
    double* value;
    int size;
} Vector;

typedef double (*UniversalNormPtr)(Vector*, void*); // вектор и параметр

void printVector(Vector* v) {
    for (int i = 0; i < v->size; ++i)
        printf("%f ", v->value[i]);
    printf("\n");
    fflush(stdout);    
}

void freeVector(Vector* v) {
    free(v->value);
}

ErrorCode createVector(Vector* res, int size, ...) {
    Vector v;
    v.size = size;
    double* cords = (double*)malloc(sizeof(double) * size);
    if (!cords)
        return MALLOC_ERROR;

    va_list intList;
    va_start(intList, size);
    for (int i = 0; i < size; ++i)
        cords[i] = va_arg(intList, int);

    v.value = cords;
    *res = v;
    va_end(intList);
    return SUCCESS;
}

double normFirst(Vector* vector, void* param) {
    double maxCord = 0;
    for (int i = 0; i < vector->size; ++i) {
        double cord = fabs(vector->value[i]);
        if (cord > maxCord)
            maxCord = cord;
    }
    return maxCord;
}

double normSecond(Vector* vector, void* param) {
    double p = *(double*)param;
    double sum = 0;
    for (int i = 0; i < vector->size; ++i)
        sum += pow(fabs(vector->value[i]), p);
    return pow(sum, 1.0 / p);
}

double normThird(Vector* vector, void* param) {
    double* matrix = (double*)param;
    int n = vector->size;
    double sum = 0;
    for (int i = 0; i < n; ++i) {
        double tmp = 0;
        for (int j = 0; j < n; j++) // Ax, i*n -> по столбцам, j -> по строкам
            tmp += vector->value[j] * matrix[i*n + j];
        sum += (fabs(tmp) * vector->value[i]);
    }
    return sqrt(sum);
}

Vector** getNorm(int cntVectors, int cntNorms, ...) {
    Vector** result = (Vector**)malloc(cntNorms * sizeof(Vector*));
    if (!result)
        return NULL;

    Vector* vectors = (Vector*)malloc(cntVectors * sizeof(Vector));
    if (vectors == NULL) {
        free(result);
        return NULL;
    }

    va_list args;
    va_start(args, cntNorms);
    for (int i = 0; i < cntVectors; ++i)
        vectors[i] = va_arg(args, Vector);

    for (int i = 0; i < cntNorms; ++i) {
        // локальный результат заканчивается терминатором NULL
        Vector* localResult = (Vector*)malloc((cntVectors + 1) * sizeof(Vector));
        if (!localResult) {
            free(vectors);
            for (int j = 0; j < i; ++j)
                free(result[j]);
            free(result);
            va_end(args);
            return NULL;
        }
        UniversalNormPtr Norm = va_arg(args, UniversalNormPtr);
        void* param = va_arg(args, void*);

        double maxNorm = -1;
        int count = 0;
        for (int j = 0; j < cntVectors; j++) {
            Vector v = vectors[j];
            double norm = Norm(&v, param);
            if (norm > maxNorm) {
                maxNorm = norm;
                count = 0;
            }
            if (norm == maxNorm)
                localResult[count++] = v;
        }
        localResult[count].value = NULL;
        Vector* ptr = (Vector*)realloc(localResult, (count + 1) * sizeof(Vector));
        if (!ptr) {
            free(vectors);
            free(localResult);
            for (int j = 0; j < i; ++j)
                free(result[j]);
            free(result);
            va_end(args);
            return NULL;
        }
        result[i] = ptr;
    }
    va_end(args);
    free(vectors);
    return result;
}

int main() {
    srand(time(NULL));
    Vector v1, v2, v3;
    createVector(&v1, 3, 2, 4, 6);
    createVector(&v2, 3, 1, 9, 2);
    createVector(&v3, 3, 0, -10, 2);
    printVector(&v1);
    printVector(&v2);
    printVector(&v3);

    double p = 2;
    double A[] = {
        4, 1, 2,
        1, 5, 3,
        2, 3, 6 };

    Vector** result = getNorm(3, 3, v1, v2, v3, &normFirst, NULL, &normSecond, &p, &normThird, A);
    if (!result) {
        printf("%s\n", errorMessages[MALLOC_ERROR]);
        fflush(stdout);
        return MALLOC_ERROR;
    }

    for (int i = 0; i < 3; ++i) {
        printf("Норма#%d:\n", i + 1);
        for (int j = 0; result[i][j].value != NULL; ++j)
            printVector(&(result[i][j]));
        free(result[i]);
    }
    freeVector(&v1);
    freeVector(&v2);
    freeVector(&v3);
    return SUCCESS;
}