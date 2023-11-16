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
    FORK_ERROR,
    RETRY_ERROR,
    MESSAGES_SENDING_ERROR,
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
    "Проблемки с запуском дочернего процесса 🐣",
    "Слишком много попыток пересоздать очередь 🔄",
    "Не удаётся отправить сообщение в нужный канал, грустно 📻",
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

void generateEmojiSymbol(char* output, size_t* i, char const *emoji, const size_t emjiSz);
void generateEmojiSubsting(char* output, size_t* i, char* substring, int gapLen);
void generateEmojiString(char* output, char left[3], char middle[1], char right[3]);
ErrorCode userSession(int userId);
ErrorCode removeQueue(key_t key);
ErrorCode generateQueues(int channel, int tryes, int* fromServerQueue, int* toServerQueue);


void generateMessage(char* answer, int answLen, char* messageTextPtr) {
    if (answer == NULL || messageTextPtr == NULL)
        return;
    for (int i = 0; i < BUFFER_SIZE; ++i) {
        if (i < answLen) {
            messageTextPtr[i] = answer[i];
        } else {
            messageTextPtr[i] = '\0';
        }
    }
}

int main() {
    printf("🖥️ server:\n");
    fflush(stdout);
    int fromServerQueue, toServerQueue;
    ErrorCode code = generateQueues(1, 0, &fromServerQueue, &toServerQueue);
    switch (code) {
        default:
            printf("%s\n", errorMessages[code]);
            fflush(stdout);
            return code;
        case SUCCESS:
            break;
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
                break;
            }
        }

        if (userId == -1) {
            printf("Пользователь %ld не найден\n", message.type);
            fflush(stdout);
            char answer[] = "Пользователь не найден";
            int answLen = sizeof(answer);
            (void)generateMessage(answer, answLen, message.text);
        } else {
            printf("Пользователь %ld найден\n", message.type);
            fflush(stdout);
            char answer[] = "Успешная авторизация";
            int answLen = sizeof(answer);
            (void)generateMessage(answer, answLen, message.text);
            // отдельная сессия для каждого пользователя
            int pid = fork();
            if (pid == 0) {
                //userSession(); // todo
                return SUCCESS;
            } else if (pid == -1) {
                printf("%s\n", errorMessages[FORK_ERROR]);
                fflush(stdout);
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

    // (void)msgctl(toServerQueue, IPC_RMID, NULL);
    // (void)msgctl(fromServerQueue, IPC_RMID, NULL);
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

ErrorCode userSession(int userId) {
    int fromServerQueue, toServerQueue;
    ErrorCode code = generateQueues(1 + userId, 0, &fromServerQueue, &toServerQueue);
    switch (code) {
        default:
            printf("%s\n", errorMessages[code]);
            fflush(stdout);
            return code;
        case SUCCESS:
            break;
    }

    //apple 🍏, goat 🐐, wolf 🐺, waves and coasts 🌳xxx🌊wx🛶🧑‍🌾w🌊xxx🌳
    char leftCoast[3] = {'A', 'G', 'W'};
    char boat[2] = {'_', 'L'};
    char rightCoast[3] = {'_', '_', '_'};

    struct MsgBuffer message;
    message.type = 1;
    int lineLen = sizeof(message.text);
    for (int i = 0; i < lineLen; ++i) message.text[i] = '\0';
    int drawFlag = 0;

    while (1) {
        switch (msgrcv(fromServerQueue, &message, sizeof(message), 0, 0)) {
            default:
                break;
            case -1:
                printf("%s\n", errorMessages[MESSAGES_READING_ERROR]);
                fflush(stdout);
                return MESSAGES_READING_ERROR;
        }
        printf("Запрос от пользователя: %s\n", message.text);
        for (int i = 0; i < lineLen; ++i) message.text[i] = '\0';

        drawFlag = 1;
        // take <obj>, put, move
        char* command = strtok(message.text, " \n");
        if (command == NULL) 
        {
            drawFlag = 0;
            char answer[] = "Пустая команда недопустима";
            int answLen = sizeof(answer);
            (void)generateMessage(answer, answLen, message.text);
        }
        else if (strcmp(command, "put") == 0)
        {
            char* coast = rightCoast;
            if (boat[0] == '_') 
            {
                drawFlag = 0;
                char answer[] = "Лодка пуста";
                int answLen = sizeof(answer);
                (void)generateMessage(answer, answLen, message.text);
            } else if (boat[1] == 'L') 
            {
                coast = leftCoast;
            } 
            switch (boat[0]) {
                default:
                    break;
                case 'A':
                    coast[0] = 'A';
                    break;
                case 'G':
                    coast[1] = 'G';
                    break;
                case 'W':
                    coast[2] = 'W';
                    break;
            }
            boat[0] = '_';
        }
        else if (strcmp(command, "move") == 0)
        {
            if (boat[1] == 'L') {
                boat[1] = 'R';
            } else {
                boat[1] = 'L';
            }
        } 
        else if (strcmp(command, "take") == 0)
        {   
            if (boat[0] != '_') {
                drawFlag = 0;
                char answer[] = "Лодка занята";
                int answLen = sizeof(answer);
                (void)generateMessage(answer, answLen, message.text);
                continue;
            }

            char* coast = rightCoast;
            if (boat[1] == 'L') {
                char* coast = leftCoast;
            }

            // take apple 🍏, goat 🐐, wolf 🐺, 
            char* object = strtok(NULL, " \n");
            if (object == NULL) 
            {
                drawFlag = 0;
                char answer[] = "Объект не указан";
                int answLen = sizeof(answer);
                (void)generateMessage(answer, answLen, message.text);
            } else if (strcmp(command, "apple") == 0) {
                boat[0] = 'A';
                coast[0] = '_';
            } else if (strcmp(command, "goat") == 0) {
                boat[0] = 'G';
                coast[1] = '_';
            } else if (strcmp(command, "wolf") == 0) {
                boat[0] = 'W';
                coast[2] = '_';
            } else {
                drawFlag = 0;
                char answer[] = "Объект не распознан";
                int answLen = sizeof(answer);
                (void)generateMessage(answer, answLen, message.text);
            }
        } else {
            drawFlag = 0;
            char answer[] = "Такой команды нет";
            int answLen = sizeof(answer);
            (void)generateMessage(answer, answLen, message.text);
        }

        if (drawFlag) {
            char answer[BUFFER_SIZE];
            (void)generateEmojiString(answer, leftCoast, boat, rightCoast);
            int answLen = sizeof(answer);
            (void)generateMessage(answer, answLen, message.text);
        }   

        switch (msgsnd(toServerQueue, &message, sizeof(message), 0)) {
            default:
                break;
            case -1:
                printf("%s\n", errorMessages[MESSAGES_SENDING_ERROR]);
                fflush(stdout);
                return MESSAGES_SENDING_ERROR;
        }
        for (int i = 0; i < lineLen; ++i) message.text[i] = '\0';
    }
}

void generateEmojiSymbol(char* output, size_t* i, char const *emoji, const size_t emjiSz) {
    if (*i >= BUFFER_SIZE) {
        output[BUFFER_SIZE - 1] = '\0';
        return;
    }
    for (size_t j = 0; j+1 < emjiSz; ++j) {
        output[(*i)++] = emoji[j];
    }
}

void generateEmojiSubsting(char* output, size_t* i, char* substring, int gapLen) {
    for (int k = 0; k < gapLen; ++k) {
        switch (substring[k]) {
            default:
                break;
            case 'A':
                generateEmojiSymbol(output, i, "🍏", sizeof("🍏"));
                break;
            case 'G':
                generateEmojiSymbol(output, i, "🐐", sizeof("🐐"));
                break;
            case 'W':
                generateEmojiSymbol(output, i, "🐺", sizeof("🐺"));
                break;
            case '_':
                generateEmojiSymbol(output, i, "_", sizeof("_"));
                break;
        }
    }
}

void generateEmojiString(char* output, char left[3], char middle[2], char right[3]) {
    size_t i = 0;
    generateEmojiSymbol(output, &i, "🌳", sizeof("🌳"));
    generateEmojiSymbol(output, &i, " ", sizeof(" "));

    generateEmojiSubsting(output, &i, left, 3);

    generateEmojiSymbol(output, &i, " ", sizeof(" "));
    generateEmojiSymbol(output, &i, "🌊", sizeof("🌊"));
    if (middle[1] == 'R') generateEmojiSymbol(output, &i, "🌊", sizeof("🌊"));
    generateEmojiSymbol(output, &i, " ", sizeof(" "));

    generateEmojiSubsting(output, &i, middle, 1);

    generateEmojiSymbol(output, &i, "🛶", sizeof("🛶"));
    generateEmojiSymbol(output, &i, "🤠", sizeof("🤠"));
    generateEmojiSymbol(output, &i, " ", sizeof(" "));
    if (middle[1] == 'L') generateEmojiSymbol(output, &i, "🌊", sizeof("🌊"));
    generateEmojiSymbol(output, &i, "🌊", sizeof("🌊"));
    generateEmojiSymbol(output, &i, " ", sizeof(" "));

    generateEmojiSubsting(output, &i, right, 3);

    generateEmojiSymbol(output, &i, " ", sizeof(" "));
    generateEmojiSymbol(output, &i, "🌳", sizeof("🌳"));
    if (i < BUFFER_SIZE) output[i] = '\0';
}

ErrorCode generateQueues(int channel, int tryes, int* fromServerQueue, int* toServerQueue) {
    if (toServerQueue == NULL || fromServerQueue == NULL)
        return INCORRECT_INPUT;
    if (channel <= 0 || channel > 255)
        return INCORRECT_INPUT;
    if (tryes > 5)
        return RETRY_ERROR;

    key_t toServerKey = ftok("server_key.txt", channel);
    key_t fromServerKey = ftok("client_key.txt", channel);
    if (toServerKey == -1 || fromServerKey == -1) {
        return KEY_ERROR;
    }

    int tempToServerQueue = msgget(toServerKey, 0666 | IPC_CREAT);
    int tempFromServerQueue = msgget(fromServerKey, 0666 | IPC_CREAT);
    if (tempToServerQueue == -1 || tempFromServerQueue == -1) {
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
        generateQueues(channel, ++tryes, fromServerQueue, toServerQueue);
    }

    *toServerQueue = tempToServerQueue;
    *fromServerQueue = tempFromServerQueue;
    return SUCCESS;
}