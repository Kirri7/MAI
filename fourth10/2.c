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

typedef struct {
    int *data;
    size_t size;
    size_t capacity;
} IntVector;

ErrorCode createIntVector(IntVector *vec) {
    if (!vec)
        return INCORRECT_INPUT;
    vec->data = (int*)malloc(1 * sizeof(int));
    if (!vec->data)
        return MALLOC_ERROR;
    vec->size = 0;
    vec->capacity = 1;
    return SUCCESS;
}

ErrorCode pushIntVector(IntVector *vec, const int value) {
    if (!vec)
        return INCORRECT_INPUT;
    if (vec->size >= vec->capacity) {
        size_t newCapacity = vec->capacity * 2;
        int *newData = (int*)realloc(vec->data, newCapacity * sizeof(int));
        if (!newData)
            return MALLOC_ERROR;
        vec->data = newData;
        vec->capacity = newCapacity;
    }
    vec->data[vec->size++] = value;
    return SUCCESS;
}

ErrorCode getIntVector(const IntVector *vec, const size_t index, int *res) {
    if (!res || !vec || index >= vec->size || index < 0)
        return INCORRECT_INPUT;
    *res = vec->data[index];
    return SUCCESS;
}

void freeIntVector(IntVector *vec) {
    if (!vec)
        return;
    free(vec->data);
    vec->data = NULL;
    vec->size = 0;
    vec->capacity = 0;
}

ErrorCode clearIntVector(IntVector *vec) {
    freeIntVector(vec);
    return createIntVector(vec);
}

void printIntVector(IntVector *vec) {
    if (!vec)
        return;

    for (size_t i = 0; i < vec->size; ++i) {
        int res;
        getIntVector(vec, i, &res);
        printf("%d\n", res);
    }
    fflush(stdout);
}

int codeCheck(ErrorCode code) {
    if (code != SUCCESS) {
        printf("%s\n", errorMessages[code]);
        fflush(stdout);
    }
    return code;
}

int main() {
    // todo нижний регистр
    IntVector vec;
    createIntVector(&vec);

    for (int i = 0; i < 20; i++) {
        pushIntVector(&vec, i * 10);
    }

    printIntVector(&vec);

    freeIntVector(&vec);

    return 0;
}