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

const int A_SIZE = 15;

void printHelp();
int randInt(int a, int b);
void swapMaxMin(int *arr);
void printArray(int *arr);

int main(int argc, char *argv[]) {
    if (argc != 5) {
        printHelp();
        return 0;
    } else if (strcmp(argv[1], "-b") == 0 && strcmp(argv[3], "-a") == 0) {
        char *temp = argv[2];
        argv[2] = argv[4];
        argv[4] = temp;
    } else if (!(strcmp(argv[1], "-a") == 0 && strcmp(argv[3], "-b") == 0)) {
        printHelp();
        return 0;
    }

    int a = atoi(argv[2]);
    int b = atoi(argv[4]);

    printf("Значение -a: %d\n", a);
    printf("Значение -b: %d\n", b);

    int arr[A_SIZE];
    // установка seed для rand() на основе времени 
    srand(time(NULL)); 
    for (int i = 0; i < A_SIZE; ++i) {
        arr[i] = randInt(a, b);
    }
    swapMaxMin(arr);
    printArray(arr);


    return 0;
}

void printHelp() {
    printf("Заполнение массива фиксированного размера [100] псевдослучайными числами в диапазоне "\
    "[a..b], где a, b задаются в качестве аргументов командной строки. Максимальный и минимальный "\
    "элемент будут поменяны местами.\n программа.exe -a число -b число");
}

int randInt(int a, int b) {
    if (a == b)
        return a;
    // (b - a + 1) - длина диапазона
    // a+ - начальная точка диапазона
    // position - отношение случ_числа к максимальному инту
    // без (int) левое число диапозона встречается только при rand() = 0
    // если сделать лицо попроще, то 'return a + rand() % (b - a + 1)'
    double position = rand() / (RAND_MAX + 1.0);
    return  a + (int)(position * (b - a + 1));
}

void swapMaxMin(int *arr) {
    int minPos = 0;
    int maxPos = 0;

    for (int i = 1; i < A_SIZE; ++i) {
        if (arr[i] < arr[minPos]) {
            arr[minPos] = arr[i];
            minPos = i;
        }
        if (arr[i] > arr[maxPos]) {
            arr[maxPos] = arr[i];
            maxPos = i;
        }
    }

    DEBUG_PRINT("max = %d at %d, min = %d at %d\n", arr[maxPos], maxPos, arr[minPos], minPos);
    int temp = arr[maxPos];
    arr[maxPos] = arr[minPos];
    arr[minPos] = temp;
}

void printArray(int *arr) {
    for (int i = 0; i < A_SIZE; ++i) {
        printf("%d, ", arr[i]);
    }
    printf("\n");
}