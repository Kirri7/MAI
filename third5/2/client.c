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
    MESSAGES_SENDING_ERROR,
    KEY_ERROR,
    BAD_AUTH,
    RETRY_ERROR,
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
    "Очередь не читается, грустно 😿",
    "Не удаётся отправить сообщение в нужный канал, грустно 📻",
    "Проблемы с ключом 🗿",
    "",
    "Слишком много попыток пересоздать очередь 🔄",
    "Неизвестная ошибка, что-то пошло не так 🫢"
};

struct MsgBuffer {
    long type; // id пользователя
    char text[BUFFER_SIZE];
};

ErrorCode generateQueues(int channel, int tryes, int* fromServerQueue, int* toServerQueue);
ErrorCode auth(int userId);

int main() {
    printf("⌨️ client:\n");
    fflush(stdout);
    int userId = 32; // todo

    ErrorCode code = auth(userId);
    switch (code) {
        default:
            printf("%s\n", errorMessages[code]);
            fflush(stdout);
            return code;
        case SUCCESS:
            break;
    }

    int fromServerQueue, toServerQueue;
    //sleep(5); // на случай, если клиент быстрее сервера подключается к сообщениям
    code = generateQueues(1 + userId, 0, &fromServerQueue, &toServerQueue);
    switch (code) {
        default:
            printf("%s\n", errorMessages[code]);
            fflush(stdout);
            return code;
        case SUCCESS:
            break;
    }


    //fileIn = fopen(argv[1], "r");
    FILE* file = fopen("commands1.txt", "r");
    if (file == NULL) {
        printf("%s\n", errorMessages[FILE_OPENING_ERROR]);
        fflush(stdout);
        return FILE_OPENING_ERROR;
    }

    struct MsgBuffer message;
    message.type = 1;
    int lineLen = sizeof(message.text);
    for (int i = 0; i < lineLen; ++i) message.text[i] = '\0';

    while (fgets(message.text, lineLen, file) != NULL) {
        printf("⌨️ -> %s", message.text);
        fflush(stdout);
        switch (msgsnd(toServerQueue, &message, sizeof(message), 0)) {
            default:
                break;
            case -1:
                printf("%s\n", errorMessages[MESSAGES_SENDING_ERROR]);
                fflush(stdout);
                return MESSAGES_SENDING_ERROR;
        }
        for (int i = 0; i < lineLen; ++i) message.text[i] = '\0';

        switch (msgrcv(fromServerQueue, &message, sizeof(message), 0, 0)) {
            default:
                break;
            case -1:
                printf("%s\n", errorMessages[MESSAGES_READING_ERROR]);
                fflush(stdout);
                return MESSAGES_READING_ERROR;
        }
        printf("🖥️ -> %s\n", message.text);
        for (int i = 0; i < lineLen; ++i) message.text[i] = '\0';

        if (message.type == 2) {
            return SUCCESS;
        }
    }

    message.type = 2;
    switch (msgsnd(toServerQueue, &message, sizeof(message), 0)) {
        default:
            break;
        case -1:
            printf("%s\n", errorMessages[MESSAGES_SENDING_ERROR]);
            fflush(stdout);
            return MESSAGES_SENDING_ERROR;
    }

    return 0;
}

ErrorCode generateQueues(int channel, int tryes, int* fromServerQueue, int* toServerQueue) {
    if (toServerQueue == NULL || fromServerQueue == NULL) {
        return INCORRECT_INPUT;
    }
    if (channel <= 0 || channel > 255)
        return INCORRECT_INPUT;
    if (tryes > 5)
        return RETRY_ERROR;

    key_t toServerKey = ftok("server_key.txt", channel);
    key_t fromServerKey = ftok("client_key.txt", channel);
    if (toServerKey == -1 || fromServerKey == -1) {
        return KEY_ERROR;
    }

    int tempToServerQueue = msgget(toServerKey, 0666);
    int tempFromServerQueue = msgget(fromServerKey, 0666);
    if (tempToServerQueue == -1 || tempFromServerQueue == -1) {
        printf("Очереди ещё существуют, ждём...\n");
        return generateQueues(channel, ++tryes, fromServerQueue, toServerQueue);
    } else {
        *toServerQueue = tempToServerQueue;
        *fromServerQueue = tempFromServerQueue;
    }

    return SUCCESS;
}

ErrorCode auth(int userId) {
    int fromServerQueue, toServerQueue;
    ErrorCode code = generateQueues(1, 0, &fromServerQueue, &toServerQueue);
    switch (code) {
        default:
            return code;
        case SUCCESS:
            break;
    }

    struct MsgBuffer message;

    message.type = userId;
    printf("Авторизация как пользователь %ld...\n", message.type);
    fflush(stdout);

    switch (msgsnd(toServerQueue, &message, sizeof(message), 0)) {
        default:
            break;
        case -1:
            return MESSAGES_SENDING_ERROR;
    }

    switch (msgrcv(fromServerQueue, &message, sizeof(message), message.type, 0)) {
        default:
            break;
        case -1:
            return MESSAGES_READING_ERROR;
    }

    if (strcmp(message.text, "Пользователь не найден") == 0) {
        printf("Пользователь %ld не найден\n", message.type);
        fflush(stdout);
        return BAD_AUTH;
    } else {
        printf("Пользователь %ld найден\n", message.type);
        fflush(stdout);
    }
    return SUCCESS;
}