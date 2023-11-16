#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/wait.h>

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
    MESSAGES_REMOVE_ERROR,
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
    "Не удалось удалить очередь сообщений, грустно 😥",
    "Проблемы с ключом 🗿",
    "Неизвестная ошибка, что-то пошло не так 🫢"
};

struct MsgBuffer {
    long type; // id пользователя
    char text[BUFFER_SIZE];
};

struct User {
    int id;
    int channel;
};

void userSession();
ErrorCode removeQueue(key_t key);

int main() {
    printf("🖥️ server:\n");
    fflush(stdout);
    key_t toServerKey = ftok("server_key.txt", 1);
    key_t fromServerKey = ftok("client_key.txt", 1);
    if (toServerKey == -1 || fromServerKey == -1) {
        printf("%s\n", errorMessages[KEY_ERROR]);
        fflush(stdout);
        return KEY_ERROR;
    }

    int toServerQueue = msgget(toServerKey, 0666 | IPC_CREAT | IPC_EXCL);
    int fromServerQueue = msgget(fromServerKey, 0666 | IPC_CREAT | IPC_EXCL);
    if (toServerQueue == -1 || fromServerQueue == -1) {
        printf("Очереди уже существуют, удаляю...\n");
        fflush(stdout);
        ErrorCode code = SUCCESS;
        switch (removeQueue(toServerKey)) {
            default:
                break;
            case MESSAGES_OPENING_ERROR:
                code = MESSAGES_OPENING_ERROR;
                break;
            case MESSAGES_REMOVE_ERROR:
                code = MESSAGES_REMOVE_ERROR;
                break;
        }
        switch (removeQueue(fromServerKey)) {
            default:
                break;
            case MESSAGES_OPENING_ERROR:
                code = MESSAGES_OPENING_ERROR;
                break;
            case MESSAGES_REMOVE_ERROR:
                code = MESSAGES_REMOVE_ERROR;
                break;
        }
        if (code != SUCCESS) {
            printf("%s\n", errorMessages[code]);
            fflush(stdout);
            return code;
        }
        return SUCCESS;
    }

    const int userCnt = 3;
    // зарегистрированные пользователи
    struct User users[userCnt];
    users[0].id = 32;
    users[0].channel = 1 + users[0].id;
    users[1].id = 67;
    users[1].channel = 1 + users[1].id;
    users[2].id = 112;
    users[2].channel = 1 + users[2].id;

    printf("Ожидание пока кто-нибудь зайдёт\n");
    fflush(stdout);

    while (1) {
        struct MsgBuffer message;
        switch (msgrcv(toServerQueue, &message, sizeof(message), 0, 0)) {
            default:
                break;
            case -1:
                printf("%s\n", errorMessages[MESSAGES_READING_ERROR]);
                fflush(stdout);
                return MESSAGES_READING_ERROR;
        }

        int userId = -1;
        for (int i = 0; i < userCnt; ++i) {
            if (users[i].id == message.type) {
                userId = users[i].id;
            }
        }

        if (userId == -1) {
            printf("Пользователь %ld не найден\n", message.type);
            fflush(stdout);
            char answer[] = "Пользователь не найден";
            int answLen = sizeof(answer);
            for (int i = 0; i < BUFFER_SIZE; ++i) {
                if (i < answLen) {
                    message.text[i] = answer[i];
                } else {
                    message.text[i] = '\0';
                }
            }
        } else {
            printf("Пользователь %ld найден\n", message.type);
            fflush(stdout);
            char answer[] = "Успешная авторизация";
            int answLen = sizeof(answer);
            for (int i = 0; i < BUFFER_SIZE; ++i) {
                if (i < answLen) {
                    message.text[i] = answer[i];
                } else {
                    message.text[i] = '\0';
                }
            }
            // отдельная сессия для каждого пользователя
            if (fork() == 0) {
                //userSession(); // todo
                return SUCCESS;
            }
        }
        // message.type прежний, отправка в канал ответов сервера
        switch (msgsnd(fromServerQueue, &message, sizeof(message), 0)) {
            default:
                break;
            case -1:
                printf("Не удаётся отправить сообщение в канал ответов, грустно 📻\n");
                fflush(stdout);
        }
    }

    // проверять msgctl не будем, строка не исполняется
    // msgctl(toServerQueue, IPC_RMID, NULL);
    // msgctl(fromServerQueue, IPC_RMID, NULL);
    return SUCCESS;
}

ErrorCode removeQueue(key_t key) {
    int queue = msgget(key, 0666);

    if (queue == -1)
        return MESSAGES_OPENING_ERROR;

    if (msgctl(queue, IPC_RMID, NULL) == -1)
        return MESSAGES_REMOVE_ERROR;

    return SUCCESS;
}

// void userSession() {
//     struct Message message;

//     while (1) {
//         msgrcv();

//         // Обработка команд
//         printf("Получено сообщение от пользователя %d: %s\n", message.type, message.text);

//         // Отправка ответа обратно клиенту
//         msgsnd();
//     }
// }