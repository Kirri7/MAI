#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#define DEBUG 0

#ifdef DEBUG
#define DEBUG_PRINT(format, ...) printf("Debug: "); printf(format, ##__VA_ARGS__); fflush(stdout)
#else
#define DEBUG_PRINT(msg)
#endif

#define printf(format, ...) printf(format, ##__VA_ARGS__); fflush(stdout);


void printHelp();

int main(int argc, char *argv[]) {
    
    if (strcmp(argv[1], "-h") == 0) {
        printHelp();
        return 0;
    } else if (argc == 3 && strcmp(argv[1], "-e") == 0) {
        return 0;
    } else {
        printHelp();
        return 0;
    }

    return 0;
}

void printHelp() {
    printf("Вычисление значения интегралов с точностью ε, где ε (вещественное число) подаётся "\
    "программе в виде аргумента командной строки:\nпрограмма.exe -e число");
}
