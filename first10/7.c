#include <stdio.h>

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


#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("%s <флаг> имя_файлов...\n", argv[0]);
        return INCORRECT_INPUT;
    }

    char *flagArg = argv[1];

    // int flagLen = strlen(flagArg);
    // if (flagLen > 2) {
    //     printf("%s\n", errorMessages[INCORRECT_INPUT]);
    //     fflush(stdout);
    //     return INCORRECT_INPUT;
    // }

    if (flagArg[0] != '-' && flagArg[0] != '/') {
        printf("%s\n", errorMessages[INCORRECT_INPUT]);
        fflush(stdout);
        return INCORRECT_INPUT;
    }

    if (strcmp(flagArg + 1, "r") == 0 && argc == 5) {
        // Обработка флага -r с тремя параметрами
        // char *inputFile1 = argv[2];
        // char *inputFile2 = argv[3];
        // char *outputFile = argv[4];
    } else if (strcmp(flagArg + 1, "a") == 0 && argc == 4) {
        // Обработка флага -a с двумя параметрами
        // char *inputFile = argv[2];
        // char *outputFile = argv[3];
    } else {
        printf("Неподдерживаемый флаг или неверное количество параметров 🦉\n");
        return INCORRECT_INPUT;
    }

    return SUCCESS;
}
