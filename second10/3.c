#include <asm-generic/errno-base.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>


#define DEBUG 0

#ifdef DEBUG
#define DEBUG_PRINT(format, ...) printf("Debug: "); printf(format, ##__VA_ARGS__); fflush(stdout)
#else
#define DEBUG_PRINT(msg)
#endif

#define printf(format, ...) printf(format, ##__VA_ARGS__); fflush(stdout);

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

ErrorCode getInput(char* buffer, int BUFFER_SIZE);
void searchSubstringInFiles(int filesCnt, const char* substring, ...);

int main(int argc, char *argv[]) {
    searchSubstringInFiles(3, "hello", ".conf", "lines.txt", "ok");


    printf("%s\n", errorMessages[SUCCESS]);
    return SUCCESS;
}

ErrorCode getInput(char* buffer, int BUFFER_SIZE) 
{
    //const int BUFFER_SIZE = 50;
    //char buffer[BUFFER_SIZE];
    for (int i = 0; i < BUFFER_SIZE; ++i) buffer[i] = '\0';
    if (!fgets(buffer, sizeof(char) * BUFFER_SIZE, stdin)) 
    {
        //printf("%s\n", errorMessages[INCORRECT_INPUT]);
        return INCORRECT_INPUT;
    }
    if (!(buffer[BUFFER_SIZE - 1] == '\0' || buffer[BUFFER_SIZE - 2] == '\n'))
    {
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }
    // char* input = strtok(buffer, "\n");
    // if (input == NULL) 
    // {
    //     return INCORRECT_INPUT;
    // }
    return SUCCESS;
}

void searchSubstringInFiles(int filesCnt, const char* substring, ...) {
    va_list args;
    va_start(args, substring);

    const char* currentFileName;

    for (int i = 0; i < filesCnt; ++i) {
        currentFileName = va_arg(args, const char*);
        printf("\n📄 %s:\n", currentFileName);

        FILE* file = fopen(currentFileName, "r");
        if (file == NULL) {
            fprintf(stderr, "%s\n", errorMessages[FILE_OPENING_ERROR]);
            continue;
        }

        char line[6];
        int lineLen = sizeof(line);
        for (int i = 0; i < lineLen; ++i) line[i] = '\0';

        int lineNumber = 0;
        //int subsLen = sizeof(substring); 
        int subsLen = strlen(substring);

        while (fgets(line, lineLen, file) != NULL) {
            ++lineNumber;
            //DEBUG_PRINT("line %d:\n", lineNumber);

            for (int j = 0; j < lineLen - subsLen ; ++j) 
            {
                if (line[j] == substring[0]) 
                {
                    //DEBUG_PRINT(" %d\n",j + 1);
                    int found = 1;
                    for (int k = 1; k < subsLen; ++k) 
                    {
                        if (line[j + k] != substring[k]) 
                        {
                            found = 0;
                            break;
                        }
                    }
                    if (found) 
                    {
                        printf("%d строка, %d символ\n", lineNumber, j + 1);
                    }
                }
            }
            for (int i = 0; i < lineLen; ++i) line[i] = '\0';
        }

        fclose(file);
    }

    va_end(args);
}