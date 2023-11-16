#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <unistd.h>

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

ErrorCode process_message(char *text) {
    if (text == NULL) return INCORRECT_INPUT;
    int j = 0;
    for (int i = 0; text[i] != '\0'; ++i) {
        if (!(isspace(text[i]) && isspace(text[i + 1]))) {
            text[j++] = text[i];
        }
    }
    text[j] = '\0';
    return SUCCESS;
}

int main() {
    printf("🖥️ server:\n");
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
    struct MsgBuffer message;

    while (1) {
        // приём сообщений из очереди
        msgrcv(serverQueue, &message, sizeof(message), -PRIOR_MAX, 0); 
        /*
            msgp - указатель на буфер сообщения для читаемого сообщения
            msgtyp - тип сообщения
                0 - первое в очереди, не важно какого типа
                +1 - получения сообщения этого типа
                -1 - первое сообщение с наименьшим типом, который <= abs(-1)
        */

        // Приоритет 1 -> выход
        if (message.type == 1) {
            break;
        }

        printf("pr%ld: ", message.type);
        printf("%s\n", message.text);
        fflush(stdout);
        //sleep(1); // имитация сложной обработки сообщения
        switch (process_message(message.text)) {
            default:
                break;
            case INCORRECT_INPUT:
                printf("%s\n", errorMessages[INCORRECT_INPUT]);
                fflush(stdout);
                msgctl(serverQueue, IPC_RMID, NULL);
                msgctl(clientQueue, IPC_RMID, NULL);
                return INCORRECT_INPUT;
        }

        // обратная поставка сообщений
        message.type = 1; 
        msgsnd(clientQueue, &message, sizeof(message), 0);
    }

    // удаление всех сообщений (клиент больше их не увидит)
    msgctl(serverQueue, IPC_RMID, NULL);
    msgctl(clientQueue, IPC_RMID, NULL);
    return SUCCESS;
}
