//#include <cstddef>
//#include <cstddef>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#include "number-systems.c"

#define DEBUG 0
#define BUFFER_SIZE 100

#ifdef DEBUG
#define DEBUG_PRINT(format, ...) printf("Debug: "); printf(format, ##__VA_ARGS__); fflush(stdout)
#else
#define DEBUG_PRINT(msg)
#endif

#define printf(format, ...) printf(format, ##__VA_ARGS__); fflush(stdout);

int validateCh(char ch);
void printHelp();
int guessMinNumSystem(char ch);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printHelp();
        return 0;
    }

    FILE *fileIn;
    FILE *fileOut;

    // "/полный/путь/example.txt";
    fileIn = fopen(argv[1], "r");
    fileOut = fopen(argv[2], "w");
    if (fileIn == NULL || fileOut == NULL) {
        printf("Не удалось открыть файл(ы), грустно 😥");
        return 1;
    }
    char ch;
    int inWord = 0;
    int supressZeros = 0;
    char greatestCh = '0';
    char number[BUFFER_SIZE] = "";
    int j = 0;
    while ((ch = fgetc(fileIn)) != EOF) {
        if (ch == '-' && !inWord) {
            if ((ch = fgetc(fileIn)) == EOF) {
                ch = '-';
                // break;
            } else if (validateCh(ch)) {
                fprintf(fileOut, "%c", '-');
                number[j++] = '-';
            } else {
                ch = '-';
            }
        } 
        if (ch == '0' && !inWord) {
            supressZeros = 1;
        } 
        else if ((isspace(ch) || ch == '\n')) {
            if (supressZeros) {
                supressZeros = 0;
                inWord = 1;
                fprintf(fileOut, "0");
                greatestCh = (greatestCh > ch) ? greatestCh : ch;
            }
            if (inWord) {
                int base = guessMinNumSystem(greatestCh);
                fprintf(fileOut, " %d", base);
                long temp = strtol(number, NULL, base);
                fprintf(fileOut, " %ld\n", temp);
                greatestCh = '0';
                inWord = 0;
                memset(number, '\0', BUFFER_SIZE);
                j = 0;
            }
            // пропускаем разделители
        } 
        else if (!validateCh(ch)) {
            printf("некоректная цифра или разделитель %c\n", ch);
            fclose(fileIn);
            fflush(fileOut);
            fclose(fileOut);
            return 1;
        } 
        else {
            inWord = 1;
            fprintf(fileOut, "%c", ch);
            if (j+1 >= BUFFER_SIZE) {
                printf("буффер переполнен, число в 10чной будет обрезано\n");
            } else {
                number[j++] = ch;
            }
            greatestCh = (greatestCh > ch) ? greatestCh : ch;
        }
    }
    if (supressZeros) {
        inWord = 1;
        fprintf(fileOut, "0");
        greatestCh = (greatestCh > ch) ? greatestCh : ch;
    }
    if (inWord) {
        int base = guessMinNumSystem(greatestCh);
        fprintf(fileOut, " %d", base);
        long temp = strtol(number, NULL, base);
        fprintf(fileOut, " %ld\n", temp);
    }

    fclose(fileIn);
    fflush(fileOut);
    fclose(fileOut);

    return 0;
}

void printHelp() {
    printf("Восстановление информации о СС чисел во входном файле: "\
    "входное число, минимальная сс, это число в десятичной\n"\
    " программа.exe /ввод.txt /вывод.txt\n");
}

int validateCh(char ch) {
    if ('0' <= ch && ch <= '0' + 9)
        return 1;
    if ('a' <= ch && ch <= 'a' + 25)
        return 1;
    if ('A' <= ch && ch <= 'A' + 25)
        return 1;
    return 0;
}

int guessMinNumSystem(char ch) {
    if (ch > '0' && ch <= '9')
        return ch - '0' + 1;
    if (isalpha(ch)) {
        ch = toupper(ch);
        return ch - 'A' + 10 + 1;
    }
    return 2;
}