#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
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

int isRomanDigit(char c) {
    char romanDigits[] = "IVXLCDM";

    for (int i = 0; i < strlen(romanDigits); ++i) {
        if (c == romanDigits[i]) {
            return 1; 
        }
    }
    return 0;
}

int isAlphaNumericSpace(char c) {
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
        return 1;
    }
    if (c >= '0' && c <= '9') {
        return 1;
    }
    if (c == ' ') {
        return 1;
    }
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("%s <флаг> имя_файлов...\n", argv[0]);
        return INCORRECT_INPUT;
    }

    char *flagArg = argv[1];
    int flagLen = strlen(flagArg);
    if (!((flagLen == 3 && flagArg[1] == 'n') || flagLen == 2)) {
        printf("%s\n", errorMessages[INCORRECT_INPUT]);
        fflush(stdout);
        return INCORRECT_INPUT;
    }

    if (flagArg[0] != '-' && flagArg[0] != '/') {
        printf("%s\n", errorMessages[INCORRECT_INPUT]);
        fflush(stdout);
        return INCORRECT_INPUT;
    }

    char *inputFileName = argv[2];
    FILE *inputFile = fopen(inputFileName, "r");
    if (inputFile == NULL) {
        printf("%s\n", errorMessages[FILE_OPENING_ERROR]);
        fflush(stdout);
        return FILE_OPENING_ERROR;
    }

    FILE *outputFile;
    char flag;
    if (flagArg[1] == 'n') {
        if (argc < 4) {
            printf("Недостаточно аргументов\n");
            fflush(stdout);
            fclose(inputFile);
            return INCORRECT_INPUT;
        }
        flag = flagArg[2];

        char *outputFileName = argv[3];
        outputFile = fopen(outputFileName, "w");
        if (outputFile == NULL) {
            printf("%s\n", errorMessages[FILE_OPENING_ERROR]);
            fflush(stdout);
            fclose(inputFile);
            return FILE_OPENING_ERROR;
        }
    } else {
        flag = flagArg[1];
        const size_t prefLen = sizeof("out_");
        const size_t inpLen = strlen(inputFileName);
        char outputFileName[BUFFER_SIZE] = "out_";

        if (prefLen + inpLen > sizeof(outputFileName)) {
            printf("%s\n", errorMessages[FILE_OPENING_ERROR]);
            fflush(stdout);
            fclose(inputFile);
            return FILE_OPENING_ERROR;
        }

        for (size_t i = 0; i <= strlen(inputFileName); ++i)
            outputFileName[i+prefLen-1] = inputFileName[i];
        
        outputFile = fopen(outputFileName, "w");
        if (outputFile == NULL) {
            printf("%s\n", errorMessages[FILE_OPENING_ERROR]);
            fflush(stdout);
            fclose(inputFile);
            return FILE_OPENING_ERROR;
        }
    }

    char chr;
    
    if (flag == 'd') 
    {
        while ((chr = fgetc(inputFile)) != EOF) {
            if (!isdigit(chr))
                fprintf(outputFile, "%c", chr);
        }
    } 
    else if (flag == 'i') 
    {
        int cnt = 0;
        while ((chr = fgetc(inputFile)) != EOF) {
            if (isRomanDigit(chr)) {
                ++cnt;
            } else if (chr == '\n') {
                fprintf(outputFile, "%d\n", cnt);
                cnt = 0;
            }
        }
    }
    else if (flag == 's') 
    {
        int cnt = 0;
        while ((chr = fgetc(inputFile)) != EOF) {
            if (!isAlphaNumericSpace(chr)) {
                ++cnt;
            }
            if (chr == '\n') {
                fprintf(outputFile, "%d\n", cnt);
                cnt = 0;
            }
        }
    }
    else if (flag == 'a') 
    {
        while ((chr = fgetc(inputFile)) != EOF) {
            if (!isdigit(chr)) { 
                // ASCII код символа в шестнадцатеричной записи
                fprintf(outputFile, "\\x%02X", chr);
            } else {
                fputc(chr, outputFile);
            }
        }
    }
    else
    {
        printf("Формат флага неверный\n");
        fflush(stdout);
        return INCORRECT_INPUT;
    }

    fclose(inputFile);
    fclose(outputFile);
    return SUCCESS;
}