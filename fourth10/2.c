#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

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

#define INT_SIZE 9
#define INT_FORMAT "%9s"


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
    if (!vec || index >= vec->size || index < 0) // unsigned long index
        return INCORRECT_INPUT;
    if (res)
        *res = vec->data[index];
    return SUCCESS;
}

ErrorCode popIntVector(IntVector *vec, const size_t index, int *res) {
    if (!vec || index >= vec->size || index < 0)
        return INCORRECT_INPUT;
    if (res)
        *res = vec->data[index];

    // vec->data[index] = 0;
    for (size_t i = index; i < vec->size-1; ++i) {
        vec->data[i] = vec->data[i+1];
    }
    vec->size = vec->size - 1;
    return SUCCESS;
}

ErrorCode popMoreIntVector(IntVector *vec, const size_t indexSt, const size_t indexEn) {
    if (!vec || 0 > indexSt || indexSt >= indexEn || indexEn >= vec->size)
        return INCORRECT_INPUT;

    // vec->data[index] = 0;
    int n = indexEn - indexSt;
    for (size_t i = indexSt; i < vec->size-n; ++i) {
        vec->data[i] = vec->data[i+n];
    }
    vec->size = vec->size - n;
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

void printIntVector(IntVector *vec, FILE* file) {
    if (!vec || !file)
        return;

    for (size_t i = 0; i < vec->size; ++i) {
        int res;
        getIntVector(vec, i, &res);
        fprintf(file, "%d\n", res);
    }
    fflush(file);
}

void printIntVectorOut(IntVector *vec) {
    printIntVector(vec, stdout);
}

ErrorCode codeCheck(ErrorCode code) {
    if (code != SUCCESS) {
        printf("%s\n", errorMessages[code]);
        fflush(stdout);
    }
    return code;
}

ErrorCode Load(IntVector *vec, const char *filename) {
    if (!filename || !vec)
        return INCORRECT_INPUT;

    FILE *file = fopen(filename, "r");
    if (file == NULL)
        return FILE_OPENING_ERROR;

    char buff[INT_SIZE];
    memset(buff, '\0', INT_SIZE);
    while (fscanf(file, INT_FORMAT, buff) == 1) {
        // printf("'%s'\n", buff);
        if (buff[INT_SIZE-1] != '\0') {
            char c = 'c';
            while (!isspace(c) && c != EOF)
                c = getc(file);
        }

        char *endptr;
        int num = strtol(buff, &endptr, 10);
        if (*endptr == '\0')
            pushIntVector(vec, num);
        else
            printf("'%s' - число не подходит\n", buff);;

        memset(buff, 0, INT_SIZE);
    }

    if (!feof(file)) {
        fclose(file);
        return FILE_READING_ERROR;
    }
    fclose(file);
    return SUCCESS;
}

ErrorCode Save(IntVector *vec, const char *filename) {
    if (!filename || !vec)
        return INCORRECT_INPUT;

    FILE *file = fopen(filename, "w");
    if (file == NULL)
        return FILE_OPENING_ERROR;
    
    printIntVector(vec, file);
    fclose(file);
    return SUCCESS;
}

ErrorCode Concat(IntVector *A, IntVector *B) {
    if (!A || !B)
        return INCORRECT_INPUT;
    
    size_t sizeB = B->size;
    for (size_t i = 0; i < sizeB; ++i) {
        int res;
        ErrorCode code = getIntVector(B, i, &res);
        switch (code) {
            default:
                return code;
            case SUCCESS:
                break;
        }
        code = pushIntVector(A, res);
        switch (code) {
            default:
                return code;
            case SUCCESS:
                break;
        }
    }
    return SUCCESS;
}

ErrorCode Free(IntVector *vec) {
    ErrorCode code = clearIntVector(vec);
    switch (code) {
        default:
            return code;
        case SUCCESS:
            break;
    }
    code = pushIntVector(vec, 0);
    return code;
}

ErrorCode Remove(IntVector *vec, size_t index, int count) {
    if (!vec || 0 >= count || index >= vec->size)
        return INCORRECT_INPUT;

    return popMoreIntVector(vec, index, index+count);
}

void Stats(const IntVector *vec) {
    if (!vec)
        return;
    printf("Размер: %d\n", vec->size);
    if (vec->size == 0)
        return;

    double summ = 0;
    int maxEl = vec->data[0], minEl=vec->data[0];
    size_t maxInd = 0, minInd = 0;
    int freqCnt = 0, freqElem;
    for (size_t i = 0; i < vec->size; ++i) {
        int temp;
        getIntVector(vec, i, &temp);
        summ += temp;
        if (temp >= maxEl) {
            maxEl = temp;
            maxInd = i;
        }
        if (temp <= minEl) {
            minEl = temp;
            minInd = i;
        }
        int tempCnt = 1;
        for (int j = i + 1; j < vec->size; ++j) {
            if (vec->data[i] == vec->data[j])
                ++tempCnt;
        }
        if (tempCnt > freqCnt) {
            freqElem = temp;
            freqCnt = tempCnt;
        } else if (tempCnt == freqCnt)
            freqElem = (temp > freqCnt) ? temp : freqCnt;
    }
    printf("Max: %d -> %zu\n", maxEl, maxInd);
    printf("Min: %d -> %zu\n", minEl, minInd);
    printf("Максимальная мода: %d\n", freqElem);
    double average = summ / vec->size;
    printf("СрЗнач: %f\n", average);
    int futhest = (fabs(average-maxEl) > fabs(average-minEl)) ? average-maxEl : average-minEl;
    printf("Максимальное отклонение от СрЗнач: %f\n", summ / vec->size);
    fflush(stdout);
}

void Print1(const IntVector *vec, const size_t index) {
    if (!vec || 0 > index || index >= vec->size)
        return;
    printf("%zu: %d\n", index, vec->data[index]);
    fflush(stdout);
}

void PrintN(const IntVector *vec, const size_t indexSt, const size_t indexEn) {
    if (!vec || 0 > indexSt || indexSt >= indexEn || indexEn >= vec->size)
        return;

    for (size_t i = indexSt; i <= indexEn; ++i)
        printf("%zu: %d\n", i, vec->data[i]);
    fflush(stdout);
}

void PrintAll(const IntVector *vec) {
    if (!vec)
        return;

    for (size_t i = 0; i < vec->size; ++i)
        printf("%zu: %d\n", i, vec->data[i]);
    fflush(stdout);
}

int cmpPlus(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

int cmpMinus(const void *a, const void *b) {
    return (*(int *)b - *(int *)a);
}

int cmpRandom(const void *a, const void *b) {
    int randomInt = rand() % 3;
    return 1 - randomInt;
}

void Sort(const IntVector *vec, char mode) {
    if (!vec)
        return;

    switch (mode) {
        default:
            return;
        case '+':
            return qsort(vec->data, vec->size, sizeof(int), cmpPlus);
        case '-':
            return qsort(vec->data, vec->size, sizeof(int), cmpMinus);
    }
}

void Shuffle(const IntVector *vec) {
    if (!vec)
        return;
    
    return qsort(vec->data, vec->size, sizeof(int), cmpRandom);
}

ErrorCode Rand(IntVector *vec, int count, int lb, int lr) {
    if (!vec || lb > lr || 0 >= count)
        return INCORRECT_INPUT;

    for (size_t i = 0; i < count; ++i) {
        int randomInt;
        if (lr - lb)
            randomInt = lb + rand() % (lr - lb + 1);
        else
            randomInt = lb;
        ErrorCode code = pushIntVector(vec, randomInt);
        switch (code) {
            default:
                return code;
            case SUCCESS:
                break;
        }
    }
    return SUCCESS;
}

int main() {
    srand(time(NULL));
    // todo нижний регистр
    IntVector vec;
    IntVector vecB;
    createIntVector(&vec);

    Load(&vec, "vector.txt");
    // Load(&vecB, "vector.txt");

    printIntVectorOut(&vec);

    Rand(&vec, 10, -10, 10);

    printIntVectorOut(&vec);

    freeIntVector(&vec);

    fflush(stdout);
    
    return 0;
}