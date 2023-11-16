#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>

#define BUFFER_SIZE 100
#define PRIOR_MAX 100 

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
    long type;
    char text[BUFFER_SIZE];
};

int main() {
    printf("⌨️ client:\n");
    fflush(stdout);
    //ftok - сгенерировать ключ
    key_t clientKey = ftok("client_key", 1);
    key_t serverKey = ftok("server_key", 1);
    /*
        pathname -> путь к существующему файлу или каталогу
        int номер очереди в проекте 0-255
    */
    
    if (clientKey == -1 || serverKey == -1) {
        printf("%s\n", errorMessages[KEY_ERROR]);
        fflush(stdout);
        return KEY_ERROR;
    }

    //msgget - возвращает id существующей или создаёт очередь
    int clientQueue = msgget(clientKey, 0666 | IPC_CREAT);
    int serverQueue = msgget(serverKey, 0666 | IPC_CREAT); 
    /*
        mode 0666 -> права на чтение и запись всем пользователям
        IPC_CREAT -> создание новой очереди, если ключа не существует
    */

    if (serverQueue == -1 || clientQueue == -1) {
        printf("%s\n", errorMessages[MESSAGES_OPENING_ERROR]);
        fflush(stdout);
        msgctl(serverQueue, IPC_RMID, NULL); // закрытие очереди
        msgctl(clientQueue, IPC_RMID, NULL);
        return MESSAGES_OPENING_ERROR;
    }

    FILE *file = fopen("messages.txt", "r");
    if (file == NULL) {
        printf("%s\n", errorMessages[FILE_OPENING_ERROR]);
        fflush(stdout);
        msgctl(serverQueue, IPC_RMID, NULL);
        msgctl(clientQueue, IPC_RMID, NULL);
        return FILE_OPENING_ERROR;
    }

    int cnt = 0;
    struct MsgBuffer message;
    char buffer[BUFFER_SIZE];
    for (int i = 0; i < BUFFER_SIZE; ++i) buffer[i] = '\0';

    while (fgets(buffer, sizeof(char) * BUFFER_SIZE, file) != NULL) {
        ++cnt;
        strtok(buffer, "=");
        char* prior = strtok(NULL, " ");
        if (prior == NULL) {
            printf("%s\n", errorMessages[INCORRECT_INPUT]);
            fflush(stdout);
            fclose(file);
            msgctl(serverQueue, IPC_RMID, NULL);
            msgctl(clientQueue, IPC_RMID, NULL);
            return INCORRECT_INPUT;
        }

        // Приоритет, чем меньше, тем выше
        int priorInt = atoi(prior);
        priorInt = (priorInt < 0) ? 0 : priorInt;
        priorInt = (priorInt > PRIOR_MAX - 1) ? PRIOR_MAX - 1 : priorInt;
        message.type = PRIOR_MAX - priorInt;
        
        strtok(NULL, "\"");
        char* input = strtok(NULL, "\n");
        if (input == NULL) {
            printf("%s\n", errorMessages[INCORRECT_INPUT]);
            fflush(stdout);
            fclose(file);
            msgctl(serverQueue, IPC_RMID, NULL);
            msgctl(clientQueue, IPC_RMID, NULL);
            return INCORRECT_INPUT;
        }
        for (int i = 0; i < BUFFER_SIZE; ++i) {
            if (input[i + 1] == '\0') // убираем последние "
                break;
            message.text[i] = input[i];
        }
        //msgsnd - отправка сообщений в очередь
        msgsnd(serverQueue, &message, sizeof(message), 0);
        /*
            msgp -> указатель на буфер сообщения для отправляемого сообщения
        */

        for (int i = 0; i < BUFFER_SIZE; ++i) message.text[i] = '\0';
    }

    for (int i = 0; i < cnt; ++i) {
        // приём любых сообщений из очереди
        msgrcv(clientQueue, &message, sizeof(message), 0, 0);
        printf("%s\n", message.text);
        fflush(stdout);
    }

    // Сообщение с наивысшим приоритетом 1 для завершения сервера
    // sleep(10);
    message.type = 1;
    msgsnd(serverQueue, &message, sizeof(message), 0);

    fclose(file);
    msgctl(serverQueue, IPC_RMID, NULL);
    msgctl(clientQueue, IPC_RMID, NULL);
    return SUCCESS;
}