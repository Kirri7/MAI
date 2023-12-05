#include <ctype.h>
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

ErrorCode printTree(const char *expression, FILE *file) {
    int level = 0;
    int letter= 1;

    for (int i = 0; expression[i] != '\0'; ++i) {
        if (expression[i] == '(') {
            ++level;
            ++letter;
        } else if (expression[i] == ',') {
            // fprintf(file, "\n");
            ++letter;
        } else if (expression[i] == ')') {
            --level;
        } else if (isalpha(expression[i])) {
            --letter;
            if (letter < 0)
                return INCORRECT_INPUT;
            for (int i = 0; i < level; ++i)
                fprintf(file, "--> ");
            fprintf(file, "%c\n", expression[i]);
        }
    }

    fprintf(file, "\n");
    return SUCCESS;
}

char* getInput(char buffer[BUFFER_SIZE], FILE *file) {
    memset(buffer, '\0', BUFFER_SIZE);
    return fgets(buffer, BUFFER_SIZE+1, file);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("%s <входной файл> <выходной файл>\n", argv[0]);
        return INCORRECT_INPUT;
    }

    FILE *inputFile = fopen(argv[1], "r");
    FILE *outputFile = fopen(argv[2], "w");
    if (inputFile == NULL || outputFile == NULL) {
        printf("%s\n", errorMessages[FILE_OPENING_ERROR]);
        fflush(stdout);
        return FILE_OPENING_ERROR;
    }

    char buffer[BUFFER_SIZE];
    int cnt = 1;
    while (getInput(buffer, inputFile) != NULL) {
        if (buffer[BUFFER_SIZE-1] != '\0') {
            printf("%s\n", errorMessages[OVERFLOW_ERROR]);
            fflush(stdout);
            fclose(inputFile);
            fclose(outputFile);
            return OVERFLOW_ERROR;
        }
        fprintf(outputFile, "Дерево скобочного выражения %d:\n", cnt++);
        ErrorCode code = printTree(buffer, outputFile);
        switch (code) {
            case SUCCESS:
                break;
            default:
                printf("%s\n", errorMessages[code]);
                fflush(stdout);
                fclose(inputFile);
                fclose(outputFile);
                return code;
        }
    }

    fclose(inputFile);
    fclose(outputFile);
    return SUCCESS;
}
