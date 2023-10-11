#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define DEBUG 0

#ifdef DEBUG
#define DEBUG_PRINT(format, ...) printf("Debug: "); printf(format, ##__VA_ARGS__); fflush(stdout)
#else
#define DEBUG_PRINT(msg)
#endif

#define printf(format, ...) printf(format, ##__VA_ARGS__); fflush(stdout);

//TODO сделать норм вывод

void printHelp();
int randInt(int a, int b);
void printArray(int *arr, int arrSize);
int compare(const void *a, const void *b);
int binSearchClosest(int *arr, int sizeArr, int target);

int main(int argc, char *argv[]) {
    if (argc != 1) {
        printHelp();
        return 0;
    }
    // установка seed для rand() на основе времени 
    srand(time(NULL)); 

    int sizeA = randInt(10, 10000);
    int sizeB = randInt(10, 10000);
    int sizeC = sizeA;
    int *A = (int *)malloc(sizeA * sizeof(int));
    int *B = (int *)malloc(sizeB * sizeof(int));
    int *C = (int *)malloc(sizeC * sizeof(int));

    if (A == NULL || B == NULL) {
        free(A);
        free(B);
        free(C);
        printf("Не удалось выделить память, грустно 😿\n");
        return 1;
    }

    for (int i = 0; i < sizeA; ++i) {
        A[i] = randInt(-1000, 1000);
    }
    for (int i = 0; i < sizeB; ++i) {
        B[i] = randInt(-1000, 1000);
    }

    printArray(A, sizeA);
    qsort(B, sizeB, sizeof(int), compare);
    printArray(B, sizeB);

    for (int i = 0; i < sizeC; ++i) {
        C[i] = A[i] + binSearchClosest(B, sizeB, A[i]);
    }
    
    printArray(C, sizeC);

    free(A);
    free(B);
    free(C);

    return 0;
}

void printHelp() {
    printf("Заполнение динамических массивов А и В псевдослучайного размера в диапазоне "\
    "[10..10000] псевдослучайными числами в диапазоне [-1000..1000]. Вывод динамического массива С, "\
    "где i-й элемент есть i-й элемент массива А, к которому добавлено значение ближайшего к A[i] по "\
    "значению элемента из массива В.\n программа.exe");
}

int randInt(int a, int b) {
    if (a == b)
        return a;
    // (b - a + 1) - длина диапазона
    // a+ - начальная точка диапазона
    // position - отношение случ_числа к максимальному инту
    // если сделать лицо попроще, то 'return a + rand() % (b - a + 1)'
    double position = rand() / (RAND_MAX + 1.0);
    return  a + (int)(position * (b - a + 1));
}

void printArray(int *arr, int arrSize) {
    for (int i = 0; i < arrSize; ++i) {
        printf("%d, ", arr[i]);
    }
    printf("\n");
}

int compare(const void *a, const void *b) {
    return (*(int *)a > *(int *)b);
}

int binSearchClosest(int *arr, int sizeArr, int target) {
    int l = 0;
    int r = sizeArr - 1;
    int m = (r + l) / 2;
    while (l <= r) {
        if (arr[m] > target)
            r = m - 1;
        else if (arr[m] < target)
            l = m + 1;
        else
            return arr[m]; // точное совпадение
        m = (r + l) / 2;
    }
    // сравнения левого и правого ближайших чисел
    if (m == 0)
        return arr[m];
    else if (m == sizeArr - 1)
        return arr[m];
    else if (abs(target - arr[m - 1]) <= abs(target - arr[m]))
        return arr[m - 1];
    else
        return arr[m];
}
