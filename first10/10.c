#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#include "number-systems.c"
#include "number-systems.h"

#define DEBUG 0
#define BUFFER_SIZE 100

#ifdef DEBUG
#define DEBUG_PRINT(format, ...) printf("Debug: "); printf(format, ##__VA_ARGS__); fflush(stdout)
#else
#define DEBUG_PRINT(msg)
#endif

#define printf(format, ...) printf(format, ##__VA_ARGS__); fflush(stdout);


void printHelp();
char* getInput(char buffer[BUFFER_SIZE]);

int main(int argc, char *argv[]) {
    if (argc != 1) {
        printHelp();
        return 0;
    }

    char buffer[BUFFER_SIZE] = "";
    printf("Задайте базу 🗿:\n")
    fgets(buffer, BUFFER_SIZE, stdin);
    // printf("%s\n", baseIn);
    for (int i = 0; i < 3; ++i) {
        if (buffer[i] == '\n') {
            buffer[i] = 0;
            break;
        }
    }
    int base;
    if (toDecimal(buffer, 10, &base)) {
        return 1;
    }
    printf("введено: %d\n", base);
    if (base < 2 || base > 36) {
        printf("База должна быть в корректном диапазоне\n");
        fflush(stdout);
        return 1;
    }
    
    int maxAbsNum = 0;
    while (getInput(buffer)) {
        if (strcmp(buffer, "Stop\n") == 0) {
            break;
        }
        for (int i = 0; i < BUFFER_SIZE; ++i) {
            if (islower(buffer[i])) {
                buffer[i] = '?';
                // printf("Строчные буквы не поддерживаются\n");
                break;
            }
        }

        char* endptr;
        int ovrflw;
        
        int num = abs(handmadeStrtoi(buffer, &endptr, base, &ovrflw));
        if (ovrflw)
        {
            printf("Переполнение при преобразовании числа %s", buffer);
            continue;
        }
        if (*endptr != '\0' && *endptr != '\n') 
        {
            printf("Некорректный ввод\n");
            continue;
        }
        
        maxAbsNum = (num > maxAbsNum) ? num : maxAbsNum;
    }
    int bases[] = {base, 9, 18, 27, 36};
    for (int curBase : bases) 
    {
        char *res;
        if(fromDecimal(maxAbsNum, curBase, &res)) {
            return 1;
        }
        printf("Максимальное в %dчной: %s\n", curBase, res);
        free(res);
        fflush(stdout);
    }

    return 0;
}

void printHelp() {
    printf("Нахождение максимального по модулю числа в заданной системе счисления:\nпрограмма.exe"\
    "\n основание СС [2..36]\n число1\n число2\n ...\n 'Stop'\n");
}

char* getInput(char buffer[BUFFER_SIZE]) {
    memset(buffer, '\0', BUFFER_SIZE);
    return fgets(buffer, BUFFER_SIZE, stdin);
}