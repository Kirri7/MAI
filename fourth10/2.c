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
    INCORRECT_ARG,
    INCORRECT_COMMAND,
    NO_DELIM,
    OVERFLOW_ERROR,
    MALLOC_ERROR,
    FILE_OPENING_ERROR,
    FILE_READING_ERROR,
    UNKNOWN_ERROR
} ErrorCode;

static const char* errorMessages[] = {
    "Всё хорошо, можно идти пить чай ☕",
    "Некорректный ввод, попробуйте ещё раз 🤨",
    "Некорректный аргумент, попробуйте ещё раз 🤨",
    "Некорректная команда, попробуйте ещё раз 🤨",
    "Не найдена ';', грустно 😐",
    "Произошло переполнение, ой 🤯",
    "Проблемы с выделением памяти, грустно 😐",
    "Не удалось открыть файл, грустно 😥",
    "Файл прочитан не полностью, грустно 😿",
    "Неизвестная ошибка, что-то пошло не так 🫢"
};

#define INT_SIZE 10 // ! осторожно со сравнениями < >
#define INT_FORMAT "%9s"
#define BUFFER_SIZE 100


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
        if (buff[INT_SIZE-2] != '\0') {
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

ErrorCode Copy(const IntVector *A, const size_t indexSt, const size_t indexEn, IntVector *B) {
    if (!A || !B || 0 > indexSt || indexSt >= indexEn || indexEn >= A->size)
        return INCORRECT_INPUT;

    for (size_t i = indexSt; i <= indexEn; ++i) {
        int res;
        ErrorCode code = getIntVector(A, i, &res);
        switch (code) {
            default:
                return code;
            case SUCCESS:
                break;
        }
        code = pushIntVector(B, res);
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
    return clearIntVector(vec);
}

ErrorCode Remove(IntVector *vec, size_t index, int count) {
    if (!vec || 0 >= count || index >= vec->size)
        return INCORRECT_INPUT;

    return popMoreIntVector(vec, index, index+count);
}

void Stats(const IntVector *vec) {
    if (!vec)
        return;
    printf("Размер: %zu\n", vec->size);
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
        for (size_t j = i + 1; j < vec->size; ++j) {
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
    double futhest = (fabs(average-maxEl) > fabs(average-minEl)) ? average-maxEl : average-minEl;
    printf("Максимальное отклонение от СрЗнач: %f\n", futhest);
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

ErrorCode Rand(IntVector *vec, int count, int lb, int rb) {
    if (!vec || lb > rb || 0 >= count)
        return INCORRECT_INPUT;

    for (size_t i = 0; i < count; ++i) {
        int randomInt;
        if (rb - lb)
            randomInt = lb + rand() % (rb - lb + 1);
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

ErrorCode process1Arguments(char **arg) {
    // линия уже должна быть обработана strtok
    if (!arg)
        return INCORRECT_INPUT;

    char *argument = strtok(NULL, ";");
    if (!argument) 
        return INCORRECT_ARG;
    while (isspace(*argument)) ++argument;

    *arg = argument;
    return SUCCESS;
}

ErrorCode process2Arguments(char *vecChr, char **arg2) {
    // линия уже должна быть обработана strtok
    if (!vecChr || !arg2)
        return INCORRECT_INPUT;

    char *vecName = strtok(NULL, ",");
    if (!vecName) 
        return INCORRECT_ARG;

    while (isspace(*vecName)) ++vecName; // '\0' не пробел
    if (strlen(vecName) != 1 || !isalpha(*vecName)) 
        return INCORRECT_ARG;

    *vecName = toupper(*vecName);

    char *argument2 = strtok(NULL, ";");
    if (!argument2) 
        return INCORRECT_ARG;
    
    while (isspace(*argument2)) ++argument2;

    *vecChr = *vecName;
    *arg2 = argument2;
    return SUCCESS;
}

ErrorCode process3Arguments(char *vecChr, char **arg2, char **arg3) {
    // линия уже должна быть обработана strtok
    if (!vecChr || !arg2 || !arg3)
        return INCORRECT_INPUT;

    char *vecName = strtok(NULL, ",");
    if (!vecName) 
        return INCORRECT_ARG;

    while (isspace(*vecName)) ++vecName; // '\0' не пробел
    if (strlen(vecName) != 1 || !isalpha(*vecName)) 
        return INCORRECT_ARG;
    *vecName = toupper(*vecName);

    char *argument2 = strtok(NULL, ",");
    if (!argument2) 
        return INCORRECT_ARG;
    while (isspace(*argument2)) ++argument2;

    char *argument3 = strtok(NULL, ";");
    if (!argument3) 
        return INCORRECT_ARG;
    while (isspace(*argument3)) ++argument3;

    *vecChr = *vecName;
    *arg2 = argument2;
    *arg3 = argument3;
    return SUCCESS;
}

ErrorCode process4Arguments(char *vecChr, char **arg2, char **arg3, char **arg4) {
    // линия уже должна быть обработана strtok
    if (!vecChr || !arg2 || !arg3 || !arg4)
        return INCORRECT_INPUT;

    char *vecName = strtok(NULL, ",");
    if (!vecName) 
        return INCORRECT_ARG;

    while (isspace(*vecName)) ++vecName; // '\0' не пробел
    if (strlen(vecName) != 1 || !isalpha(*vecName)) 
        return INCORRECT_ARG;
    *vecName = toupper(*vecName);

    char *argument2 = strtok(NULL, ",");
    if (!argument2) 
        return INCORRECT_ARG;
    while (isspace(*argument2)) ++argument2;

    char *argument3 = strtok(NULL, ",");
    if (!argument3) 
        return INCORRECT_ARG;
    while (isspace(*argument3)) ++argument3;

    char *argument4 = strtok(NULL, ";");
    if (!argument4) 
        return INCORRECT_ARG;
    while (isspace(*argument4)) ++argument4;

    *vecChr = *vecName;
    *arg2 = argument2;
    *arg3 = argument3;
    *arg4 = argument4;
    return SUCCESS;
}

ErrorCode doInstructions(FILE *file) {
    IntVector vecTable[26];
    for (int i = 0; i < 26; ++i) {
        ErrorCode code = createIntVector(&vecTable[i]);
        if (code != SUCCESS)
            return code;
        // vecTable[i] = NULL;
    }

    char buff[BUFFER_SIZE];
    char c;
    ErrorCode whileCode = SUCCESS;
    while ((c = fgetc(file)) != EOF) {
        // удаление одной буквы после ';'
        memset(buff, '\0', BUFFER_SIZE);
        int k = 0;
        buff[k++] = c;
        while ((c = fgetc(file)) != EOF) {
            if (c == ';' || k >= BUFFER_SIZE) {
                buff[k++] = ';';
                break;
            }
            buff[k++] = c;
        }

        char *command = strtok(buff, " \n");
        if (!command)
            break; // todo ;; + error code + memory leak
        while (isspace(*command)) ++command;

        if (strcmp(command, "Load") == 0)
        {
            char vecName;
            char *filename;
            ErrorCode code = process2Arguments(&vecName, &filename);
            if (code != SUCCESS) {
                printf("%s\n", errorMessages[code]);
                fflush(stdout);
                continue;
            }
            printf("> Load %c, %s;\n", vecName, filename);

            code = Load(&vecTable[vecName-'A'], filename);
            if (code != SUCCESS) {
                whileCode = code;
                break;
            }
        }
        else if (strcmp(command, "Save") == 0)
        {
            char vecName;
            char *filename;
            ErrorCode code = process2Arguments(&vecName, &filename);
            if (code != SUCCESS) {
                printf("%s\n", errorMessages[code]);
                fflush(stdout);
                continue;
            }
            printf("> Save %c, %s;\n", vecName, filename);

            code = Save(&vecTable[vecName-'A'], filename);
            if (code != SUCCESS) {
                whileCode = code;
                break;
            }
        }
        else if (strcmp(command, "Rand") == 0)
        {
            char vecName;
            char *countStr, *lbStr, *rbStr;
            ErrorCode code = process4Arguments(&vecName, &countStr, &lbStr, &rbStr);
            if (code != SUCCESS) {
                printf("%s\n", errorMessages[code]);
                fflush(stdout);
                continue;
            }

            printf("> Rand %c, %s, %s, %s;\n", vecName, countStr, lbStr, rbStr);
            char *endptr;
            int count = strtol(countStr, &endptr, 10);
            if (*endptr != '\0') {
                printf("%s\n", errorMessages[INCORRECT_ARG]);
                continue;
            }
            int lb = strtol(lbStr, &endptr, 10);
            if (*endptr != '\0') {
                printf("%s\n", errorMessages[INCORRECT_ARG]);
                continue;
            }
            int rb = strtol(rbStr, &endptr, 10);
            if (*endptr != '\0') {
                printf("%s\n", errorMessages[INCORRECT_ARG]);
                continue;
            }

            code = Rand(&vecTable[vecName-'A'], count, lb, rb);
            if (code != SUCCESS) {
                whileCode = code;
                break;
            }
        }
        else if (strcmp(command, "Concat") == 0)
        {
            char vecName;
            char *vecB;
            ErrorCode code = process2Arguments(&vecName, &vecB);
            if (code != SUCCESS) {
                printf("%s\n", errorMessages[code]);
                fflush(stdout);
                continue;
            }
            printf("> Concat %c, %s;\n", vecName, vecB);
            if (strlen(vecB) != 1 || !isalpha(*vecB)) {
                printf("%s\n", errorMessages[INCORRECT_ARG]);
                continue;
            }
            *vecB = toupper(*vecB);
            char vecName2 = *vecB;

            code = Concat(&vecTable[vecName-'A'], &vecTable[vecName2-'A']);
            if (code != SUCCESS) {
                whileCode = code;
                break;
            }
        }
        else if (strcmp(command, "Free") == 0)
        {
            char *vecA;
            ErrorCode code = process1Arguments(&vecA);
            if (code != SUCCESS) {
                printf("%s\n", errorMessages[code]);
                fflush(stdout);
                continue;
            }
            printf("> Free %s;\n", vecA);
            if (strlen(vecA) != 1 || !isalpha(*vecA)) {
                printf("%s\n", errorMessages[INCORRECT_ARG]);
                continue;
            }
            *vecA = toupper(*vecA);
            char vecName = *vecA;

            code = Free(&vecTable[vecName-'A']);
            if (code != SUCCESS) {
                whileCode = code;
                break;
            } 
        }
        else if (strcmp(command, "Remove") == 0)
        {
            char vecName;
            char *arg1, *arg2;
            ErrorCode code = process3Arguments(&vecName, &arg1, &arg2);
            if (code != SUCCESS) {
                printf("%s\n", errorMessages[code]);
                fflush(stdout);
                continue;
            }

            printf("> Remove %c, %s, %s;\n", vecName, arg1, arg2);
            char *endptr;
            int ind = strtol(arg1, &endptr, 10);
            if (*endptr != '\0') {
                printf("%s\n", errorMessages[INCORRECT_ARG]);
                continue;
            }
            int cnt = strtol(arg2, &endptr, 10);
            if (*endptr != '\0') {
                printf("%s\n", errorMessages[INCORRECT_ARG]);
                continue;
            }

            code = Remove(&vecTable[vecName-'A'], ind, cnt);
            if (code != SUCCESS) {
                whileCode = code;
                break;
            }
        }
        else if (strcmp(command, "Copy") == 0)
        {
            char vecName;
            char *indStStr, *indEnStr, *vecB;
            ErrorCode code = process4Arguments(&vecName, &indStStr, &indEnStr, &vecB);
            if (code != SUCCESS) {
                printf("%s\n", errorMessages[code]);
                fflush(stdout);
                continue;
            }

            printf("> Copy %c, %s, %s, %s;\n", vecName, indStStr, indEnStr, vecB);
            char *endptr;
            int indSt = strtol(indStStr, &endptr, 10);
            if (*endptr != '\0') {
                printf("%s\n", errorMessages[INCORRECT_ARG]);
                continue;
            }
            int indEn = strtol(indEnStr, &endptr, 10);
            if (*endptr != '\0') {
                printf("%s\n", errorMessages[INCORRECT_ARG]);
                continue;
            }
            if (strlen(vecB) != 1 || !isalpha(*vecB)) {
                printf("%s\n", errorMessages[INCORRECT_ARG]);
                continue;
            }
            *vecB = toupper(*vecB);
            char vecName2 = *vecB;

            code = Copy(&vecTable[vecName-'A'], indSt, indEn, &vecTable[vecName2-'A']);
            if (code != SUCCESS) {
                whileCode = code;
                break;
            }
        }
        else if (strcmp(command, "Sort") == 0)
        {
            char *vecA;
            ErrorCode code = process1Arguments(&vecA);
            if (code != SUCCESS) {
                printf("%s\n", errorMessages[code]);
                fflush(stdout);
                continue;
            }
            printf("> Sort %s;\n", vecA);
            if (strlen(vecA) != 2 || !isalpha(*vecA) || (vecA[1] != '+' && vecA[1] != '-')) {
                printf("%s\n", errorMessages[INCORRECT_ARG]);
                continue;
            }
            *vecA = toupper(*vecA);
            char vecName = *vecA;
            char mode = vecA[1];

            Sort(&vecTable[vecName-'A'], mode);
        }
        else if (strcmp(command, "Shuffle") == 0)
        {
            char *vecA;
            ErrorCode code = process1Arguments(&vecA);
            if (code != SUCCESS) {
                printf("%s\n", errorMessages[code]);
                fflush(stdout);
                continue;
            }
            printf("> Shuffle %s;\n", vecA);
            if (strlen(vecA) != 1 || !isalpha(*vecA)) {
                printf("%s\n", errorMessages[INCORRECT_ARG]);
                continue;
            }
            *vecA = toupper(*vecA);
            char vecName = *vecA;

            Shuffle(&vecTable[vecName-'A']);
        }
        else if (strcmp(command, "Stats") == 0)
        {
            char *vecA;
            ErrorCode code = process1Arguments(&vecA);
            if (code != SUCCESS) {
                printf("%s\n", errorMessages[code]);
                fflush(stdout);
                continue;
            }
            printf("> Stats %s;\n", vecA);
            if (strlen(vecA) != 1 || !isalpha(*vecA)) {
                printf("%s\n", errorMessages[INCORRECT_ARG]);
                continue;
            }
            *vecA = toupper(*vecA);
            char vecName = *vecA;

            Stats(&vecTable[vecName-'A']);
        }
        else if (strcmp(command, "Print") == 0)
        {
            char vecName;
            char *argStr;
            ErrorCode code = process2Arguments(&vecName, &argStr);
            if (code != SUCCESS) {
                printf("%s\n", errorMessages[code]);
                fflush(stdout);
                continue;
            }
            printf("> Print %c, %s;\n", vecName, argStr);

            if (strcmp(argStr, "all") == 0) {
                PrintAll(&vecTable[vecName-'A']);
                continue;
            }
            char *endptr;
            int ind = strtol(argStr, &endptr, 10);
            if (*endptr != '\0' && *endptr != ',') {
                printf("%s\n", errorMessages[INCORRECT_ARG]);
                continue;
            }
            if (*endptr == '\0')
                Print1(&vecTable[vecName-'A'], ind);

            char *cntStr = endptr + 1;
            int ind2 = strtol(cntStr, &endptr, 10);
            if (*endptr != '\0') {
                printf("%s\n", errorMessages[INCORRECT_ARG]);
                continue;
            }
            PrintN(&vecTable[vecName-'A'], ind, ind2);
        }
        else 
        {
            if (*command != '\0')
                printf("%s\n", errorMessages[INCORRECT_COMMAND]);
            continue;
        }
    }

    // char *endptr;
    // int num = strtol(buff, &endptr, 10);
    // if (*endptr == '\0')
    for (int i = 0; i < 26; ++i) 
        freeIntVector(&vecTable[i]);
    if (whileCode != SUCCESS)
        return whileCode;
    if (!feof(file))
        return FILE_READING_ERROR;
    return SUCCESS;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("%s <имя_файла>\n", argv[0]);
        return INCORRECT_INPUT;
    }

    srand(time(NULL));
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("%s\n", errorMessages[FILE_OPENING_ERROR]);
        fflush(stdout);
        return FILE_OPENING_ERROR;
    }

    ErrorCode code = doInstructions(file);
    fclose(file);
    
    return 0;
}