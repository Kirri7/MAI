#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#include "number-systems.c"
#include "number-systems.h"

#define DEBUG 0

#ifdef DEBUG
#define DEBUG_PRINT(format, ...) printf("Debug: "); printf(format, ##__VA_ARGS__); fflush(stdout)
#else
#define DEBUG_PRINT(msg)
#endif

#define printf(format, ...) printf(format, ##__VA_ARGS__); fflush(stdout);


void printHelp();

int main(int argc, char *argv[]) {
    if (argc != 1) {
        printHelp();
        return 0;
    }

    char baseIn[3];
    printf("Выдайте базу 🗿:\n")
    fgets(baseIn, sizeof(baseIn), stdin);
    printf("%s\n", baseIn);
    for (int i = 0; i < 3; ++i) {
        if (baseIn[i] == '\n') {
            baseIn[i] = 0;
            break;
        }
    }
    int base;
    if (toDecimal(baseIn, 10, &base)) {
        return 1;
    }
    printf("%d\n", base);


    return 0;
}

void printHelp() {
    printf("Нахождение максимального по модулю числа в заданной системе счисления:\nпрограмма.exe"\
    "\n основание СС [2..36]\n число1\n число2\n ...\n 'Stop'\n");
}
