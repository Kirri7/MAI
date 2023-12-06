#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

#define BUFFER_SIZE 256

typedef struct{
    unsigned int id;
    char firstName[50];
    char lastName[50];
    double salary;
} Employee;

char* getInput(char buffer[BUFFER_SIZE], FILE *file) {
    memset(buffer, '\0', BUFFER_SIZE);
    return fgets(buffer, BUFFER_SIZE+1, file);
}

void closeFiles(FILE *fileIn, FILE *fileOut) {
    if (fileIn != NULL)
        fclose(fileIn);
    if (fileOut != NULL)
        fclose(fileOut);
}

int compare(const void *x, const void *y) {
    Employee W1 = *(Employee*)x;
    Employee W2 = *(Employee*)y;

    if (W1.salary - W2.salary != 0) // здесь могла быть epsilon
        return W1.salary - W2.salary;

    if (strcmp(W1.lastName, W2.lastName) != 0)
        return strcmp(W1.lastName, W2.lastName);
    
    if (strcmp(W1.lastName, W2.lastName) != 0)
        return strcmp(W1.lastName, W2.lastName);

    return W1.id - W2.id;
}

int cmpA(const void *x, const void *y) {
    return compare(x, y);
}

int cmpD(const void *x, const void *y) {
    return compare(y, x); // или -1 * compare
}

ErrorCode getData(FILE *fileIn, FILE *fileOut, Employee **database, int *dataSize) {
    if (!fileIn || !fileOut || !database || !dataSize)
        return INCORRECT_INPUT;

    Employee *workers = (Employee*)malloc(1);
    if (workers == NULL) {
        return MALLOC_ERROR;
    }
    int wrksCnt = 0;
    char buffer[BUFFER_SIZE] = "";
    while (getInput(buffer, fileIn) != NULL) {
        if (buffer[BUFFER_SIZE-1] != '\0') {
            free(workers);
            return OVERFLOW_ERROR;
        }

        Employee worker;
        memset(worker.firstName, '\0', sizeof(worker.firstName));
        memset(worker.lastName, '\0', sizeof(worker.lastName));
        char empty;
        if (sscanf(buffer, "%u %s %s %lf %c", &worker.id, worker.firstName, 
                worker.lastName, &worker.salary, &empty) != 4 || worker.salary < 0) 
        {
            printf("%s\n", errorMessages[INCORRECT_INPUT]);
            printf("%s", buffer);
            fflush(stdout);
            continue;
        }
        worker.salary = fabs(worker.salary);

        Employee *tempPtr = (Employee*)realloc(workers, sizeof(Employee) * ++wrksCnt);
        if (tempPtr == NULL) {
            --wrksCnt;
            if (workers != NULL) // на всякий случай
                free(workers);
            return MALLOC_ERROR;
        }
        workers = tempPtr;

        workers[wrksCnt-1] = worker;
    }

    *database = workers;
    *dataSize = wrksCnt;
    return SUCCESS;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("%s <входной файл> <флаг> <выходной файл>\n", argv[0]);
        return INCORRECT_INPUT;
    }

    char *flagArg = argv[2];
    if (flagArg[0] != '-' && flagArg[0] != '/') {
        printf("%s\n", errorMessages[INCORRECT_INPUT]);
        fflush(stdout);
        return INCORRECT_INPUT;
    }

    ++flagArg;
    if (strcmp(flagArg, "a") != 0 && strcmp(flagArg, "d") != 0) {
        printf("Неподдерживаемый флаг или неверное количество параметров 🦉\n");
        return INCORRECT_INPUT;
    }

    FILE *fileIn = fopen(argv[1], "r");
    FILE *fileOut = fopen(argv[3], "w");
    if (fileIn == NULL || fileOut == NULL) {
        printf("%s\n", errorMessages[FILE_OPENING_ERROR]);
        fflush(stdout);
        closeFiles(fileIn, fileOut);
        return FILE_OPENING_ERROR;
    }

    Employee *workers;
    int wrksCnt;
    ErrorCode code = getData(fileIn, fileOut, &workers, &wrksCnt);
    switch (code) {
        case SUCCESS:
            break;
        default:
            printf("%s\n", errorMessages[code]);
            fflush(stdout);
            closeFiles(fileIn, fileOut);
            return code;
    }

    if (strcmp(flagArg, "a") == 0) {
        qsort(workers, wrksCnt, sizeof(Employee), cmpA);
    } else if (strcmp(flagArg, "d") == 0) {
        qsort(workers, wrksCnt, sizeof(Employee), cmpD);
    }

    for (int i = 0; i < wrksCnt; ++i) {
        Employee worker = *(workers+i);
        fprintf(fileOut, "%u %s %s %lf\n", 
            worker.id, 
            worker.firstName, 
            worker.lastName, 
            worker.salary);
    }

    free(workers);
    closeFiles(fileIn, fileOut);
    return SUCCESS;
}