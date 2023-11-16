#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/msg.h>

#define BUFFER_SIZE 100

typedef enum {
    SUCCESS = 0,
    INCORRECT_INPUT,
    OVERFLOW_ERROR,
    MALLOC_ERROR,
    FILE_OPENING_ERROR,
    FILE_READING_ERROR,
    MESSAGES_OPENING_ERROR,
    MESSAGES_READING_ERROR,
    KEY_ERROR,
    UNKNOWN_ERROR
} ErrorCode;

static const char* errorMessages[] = {
    "Всё хорошо, можно идти пить чай ☕",
    "Некорректный ввод, попробуйте ещё раз 🤨",
    "Произошло переполнение, ой 🤯",
    "Проблемы с выделением памяти, грустно 😐",
    "Не удалось открыть файл, грустно 😥",
    "Файл прочитан не полностью, грустно 😿",
    "Не удалось открыть очередь сообщений, грустно 😥",
    "Очередь прочитана не полностью, грустно 😿",
    "Проблемы с ключом 🗿",
    "Неизвестная ошибка, что-то пошло не так 🫢"
};

struct MsgBuffer {
    long type; // id пользователя
    char text[BUFFER_SIZE];
};

int main() {
    printf("⌨️ client:\n");
    fflush(stdout);
    key_t toServerKey = ftok("server_key.txt", 1);
    key_t fromServerKey = ftok("client_key.txt", 1);
    if (toServerKey == -1 || fromServerKey == -1) {
        printf("%s\n", errorMessages[KEY_ERROR]);
        fflush(stdout);
        return KEY_ERROR;
    }

    int toServerQueue = msgget(toServerKey, 0666 | IPC_CREAT);
    int fromServerQueue = msgget(fromServerKey, 0666 | IPC_CREAT);
    if (toServerQueue == -1 || fromServerQueue == -1) {
        printf("%s\n", errorMessages[MESSAGES_OPENING_ERROR]);
        fflush(stdout);
        return MESSAGES_OPENING_ERROR;
    }

    struct MsgBuffer message;

    message.type = 33; // id пользователя для авторизации
    printf("Авторизация как пользователь %ld\n", message.type);
    fflush(stdout);

    switch (msgsnd(toServerQueue, &message, sizeof(message), 0)) {
    default:
        break;
    case -1:
        printf("Не удаётся отправить сообщение в канал авторизации, грустно 📻\n");
        fflush(stdout);
    }

    switch (msgrcv(fromServerQueue, &message, sizeof(message), message.type, 0)) {
        default:
            break;
        case -1:
            printf("%s\n", errorMessages[MESSAGES_READING_ERROR]);
            fflush(stdout);
            return MESSAGES_READING_ERROR;
    }

    if (strcmp(message.text, "Пользователь не найден") == 0) {
        printf("Пользователь %ld не найден\n", message.type);
        fflush(stdout);
        return SUCCESS;
    } else {
        printf("Пользователь %ld найден\n", message.type);
        fflush(stdout);
    }

    // while (1) {
    //     // Ввод команд
    //     msgsnd();
    //     msgrcv();
    //     printf("Ответ от сервера: %s\n", message.text);
    // }

    return 0;
}
