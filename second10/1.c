#include <stddef.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
// я запрещаю вам использовать string.h 🕴️

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

ErrorCode len(const char string[], size_t *res) {
    if (!string || !res) 
        return INCORRECT_INPUT;

    size_t i;
    for(i = 0; *string != '\0'; ++i)
        ++string;

    *res = i;
    return SUCCESS;
}

ErrorCode rev(const char string[], char *res[]){
    if (!string || !res) 
        return INCORRECT_INPUT;

    size_t length = 0;
    len(string, &length);
    *res = (char*)malloc((length+1) * sizeof(char));

    if (!*res)
        return MALLOC_ERROR;
    
    for (size_t i = 0; i < length; ++i)
        (*res)[i] = string[length-i-1];
    
    (*res)[length] = '\0';
    return SUCCESS;
}

ErrorCode up(const char string[], char *res[]){
    if (!string || !res) 
        return INCORRECT_INPUT;

    size_t length = 0;
    len(string, &length);
    *res = (char*)malloc((length+1) * sizeof(char));

    if (!*res)
        return MALLOC_ERROR;

    for (size_t i = 0; i < length; i+=2)
        (*res)[i] = string[i];
    for (size_t i = 1; i < length; i+=2)
        (*res)[i] = toupper(string[i]);
    
    (*res)[length] = '\0';
    return SUCCESS;
}

ErrorCode reor(const char string[], char *res[]){
    if (!string || !res) 
        return INCORRECT_INPUT;

    size_t length = 0;
    len(string, &length);
    *res = (char*)malloc((length+1) * sizeof(char));

    if (!*res)
        return MALLOC_ERROR;

    int i = 0, j = 0;
    for (size_t k = 0; k < length; ++k) {
        if (isdigit(string[k])) ++i;
        else if (isalpha(string[k])) ++j;
    }

    j += i;
    int n = 0;
    for (size_t k = 0; k < length; ++k) {
        char ch = string[k];
        if (isdigit(ch)) (*res)[n++] = ch;
        else if (isalpha(ch)) (*res)[i++] = ch;
        else (*res)[j++] = ch;
    }
    
    (*res)[length] = '\0';
    return SUCCESS;
}

ErrorCode shaf(char *strings[], int cnt, char *res[]){
    // "1Hello?", "2f", "3f", "4f", "5ok", "6maybe"
    if (!strings || !res) 
        return INCORRECT_INPUT;

    for (int i = 0; i < cnt; ++i) {
        int j = rand() % cnt;
        char *temp = strings[i];
        strings[i] = strings[j];
        strings[j] = temp;
    }

    // for (int i = 0; i < cnt; ++i) {
    //     printf("%c\n", *strings[i]);
    //     fflush(stdout);
    // }

    size_t totalLen = 0;
    for (int i = 0; i < cnt; ++i) {
        if (!strings[i])
            return INCORRECT_INPUT;
        size_t length;
        (void)len(strings[i], &length);
        totalLen += length;
    }
    
    *res = (char*)malloc((totalLen+1) * sizeof(char));
    if (!*res)
        return MALLOC_ERROR;

    size_t k = 0;
    for (int i = 0; i < cnt; ++i) {
        size_t length;
        (void)len(strings[i], &length);
        for (size_t j = 0; j < length; ++j) {
            (*res)[k++] = strings[i][j];
        }
    }

    (*res)[totalLen] = '\0';
    return SUCCESS;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("%s <флаг> <строка> ...\n", argv[0]);
        return INCORRECT_INPUT;
    }

    char *flagArg = argv[1];
    size_t flagLen = 0;
    len(flagArg, &flagLen);
    if (flagLen != 2) {
        printf("%s\n", errorMessages[INCORRECT_INPUT]);
        fflush(stdout);
        return INCORRECT_INPUT;
    }

    if (flagArg[0] != '-' && flagArg[0] != '/') {
        printf("%s\n", errorMessages[INCORRECT_INPUT]);
        fflush(stdout);
        return INCORRECT_INPUT;
    }

    char *string = argv[2];
    
    if (flagArg[1] == 'l') 
    {
        if (argc != 3) {
            printf("... -l <string>\n");
            return INCORRECT_INPUT;
        }
        size_t answ;
        ErrorCode code = len(string, &answ);
        if (code != SUCCESS) {
            printf("%s\n", errorMessages[code]);
            fflush(stdout);
            return code;
        }
        printf("%lu\n", answ);
        fflush(stdout);
    }
    else if (flagArg[1] == 'r') 
    {
        if (argc != 3) {
            printf("... -r <string>\n");
            return INCORRECT_INPUT;
        }
        char *answ;
        ErrorCode code = rev(string, &answ);
        if (code != SUCCESS) {
            printf("%s\n", errorMessages[code]);
            fflush(stdout);
            return code;
        }
        printf("%s\n", answ);
        fflush(stdout);
        free(answ);
    }
    else if (flagArg[1] == 'u') 
    {
        if (argc != 3) {
            printf("... -u <string>\n");
            return INCORRECT_INPUT;
        }
        char *answ;
        ErrorCode code = up(string, &answ);
        if (code != SUCCESS) {
            printf("%s\n", errorMessages[code]);
            fflush(stdout);
            return code;
        }
        printf("%s\n", answ);
        fflush(stdout);
        free(answ);
    }
    else if (flagArg[1] == 'n') 
    {
        if (argc != 3) {
            printf("... -n <string>\n");
            return INCORRECT_INPUT;
        }
        char *answ;
        ErrorCode code = reor(string, &answ);
        if (code != SUCCESS) {
            printf("%s\n", errorMessages[code]);
            fflush(stdout);
            return code;
        }
        printf("%s\n", answ);
        fflush(stdout);
        free(answ);
    }
    else if (flagArg[1] == 'c') 
    {
        if (argc < 4) {
            printf("... -c <string> <u int> <str2> <str2> ...\n");
            return INCORRECT_INPUT;
        }
        char* endptr;
        unsigned int seed = strtoul(argv[3], &endptr, 10);
        if (*endptr != '\0') 
        {
            printf("Некорректный ввод\n");
            fflush(stdout);
            return INCORRECT_INPUT;
        }
        srand(seed);

        int cnt = argc - 3;
        char **strings = (char**)malloc(cnt * sizeof(char*));
        if (!strings)
        {
            printf("%s\n", errorMessages[MALLOC_ERROR]);
            fflush(stdout);
            return MALLOC_ERROR;
        }

        // for (int i = 0; i < cnt; ++i) {
        //     printf("%p\n", strings[i]);
        //     fflush(stdout);
        // }

        strings[0] = argv[2];
        for (int i = 1; i < cnt; ++i) {
            strings[i] = argv[3+i];
        }

        // for (int i = 0; i < cnt; ++i) {
        //     printf("new: %p\n", strings[i]);
        //     fflush(stdout);
        // }
        
        char *answ;
        ErrorCode code = shaf(strings, cnt, &answ);
        if (code != SUCCESS) {
            printf("%s\n", errorMessages[code]);
            fflush(stdout);
            free(strings);
            return code;
        }
        printf("%s\n", answ);
        fflush(stdout);
        free(strings);
        free(answ);
    }
    else 
    {
        printf("Формат флага неверный\n");
        fflush(stdout);
        return INCORRECT_INPUT;
    }
    
    return SUCCESS;
}