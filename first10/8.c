//#include <cstddef>
//#include <cstddef>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#include "number-systems.c"

#define DEBUG 0

#ifdef DEBUG
#define DEBUG_PRINT(format, ...) printf("Debug: "); printf(format, ##__VA_ARGS__); fflush(stdout)
#else
#define DEBUG_PRINT(msg)
#endif

#define printf(format, ...) printf(format, ##__VA_ARGS__); fflush(stdout);

typedef struct item
{
    char data;
    struct item* next;
} item;

typedef struct linked_list
{
    item* head;
} linked_list;

item* pushItem(char data);
item* readItem(item* curItem, char* data);
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
    linked_list chQueue;
    chQueue.head = NULL;
    char ch;
    int inWord = 0;
    char greatestCh = '0';
    while ((ch = fgetc(fileIn)) != EOF) {
        if (ch == '-' && !inWord) {
            if ((ch = fgetc(fileIn)) == EOF) {
                break;
            } else if (validateCh(ch)) {
                fprintf(fileOut, "%c", '-');
            }
        } 
        // if (ch == '0' && !inWord) {
        //     if ((ch = fgetc(fileIn)) == EOF) {
        //         break;
        //     } 
        //     // else if (!validateCh(ch)) {
        //     //     fprintf(fileOut, "%c", '0');
        //     // }
        // } 
        if (isspace(ch) && inWord) {
            fprintf(fileOut, " %d\n", guessMinNumSystem(greatestCh));
            greatestCh = '0';
            inWord = 0;
        } 
        else if (!validateCh(ch)) {
            printf("\n некоректная цифра %c\n", ch);
        } 
        else if (!isspace(ch)) {
            inWord = 1;
            fprintf(fileOut, "%c", ch);
            //pushItem(ch);
            greatestCh = (greatestCh > ch) ? greatestCh : ch;
        }
    }
    if (inWord) {
        fprintf(fileOut, " %d\n", guessMinNumSystem(greatestCh));
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

item* pushItem(char data) {
    item* newItem = (item*)malloc(sizeof(item));
    if (newItem == NULL) {
        printf("Проблемы с памятю, не выделяется, грустно 🫤");
        return NULL;
    }
    newItem->data = data;
    newItem->next = NULL;

    return newItem;
}

item* readItem(item* curItem, char* data) {
    if (curItem == NULL) {
        printf("Чтение NULL 🫤");
        return NULL;
    }
    *data = curItem->data;
    item* nextItem = curItem->next;
    free(curItem);
    return nextItem;
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