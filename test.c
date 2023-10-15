#include <inttypes.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <wait.h>
#include <time.h>

int validateCh(char ch) {
    const char emoji[] = "🐹";
    if ('0' <= ch && ch <= '9')
        return 1;
    if ('a' <= ch && ch <= 'z')
        return 1;
    if ('A' <= ch && ch <= 'Z')
        return 1;
    if (ch == emoji[0] || ch == emoji[1] || ch == emoji[2] || ch == emoji[3])
        return 1;
    return 0;
}


void executeCommand(char *command) {
    pid_t pid = fork();

    if (pid == -1) {
        perror("Ошибка при создании процесса");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Внутри дочернего процесса
        char *args[100];
        char *token = strtok(command, " ");
        int i = 0;

        while (token != NULL) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }

        args[i] = NULL;

        execvp(args[0], args);
        perror("Ошибка при выполнении команды");
        exit(EXIT_FAILURE);
    } else {
        // Внутри родительского процесса
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            printf("Процесс завершился с кодом %d\n", WEXITSTATUS(status));
        }
    }
}

int main() {
    int ret;
    struct tm info;
    char buffer[80];

    info.tm_year = 0 - 1901;
    info.tm_mon = 1;
    info.tm_mday = 1;
    info.tm_hour = 0;
    info.tm_min = 0;
    info.tm_sec = 0;
    //info.tm_isdst = 13;
    info.tm_isdst = -1;
    ret = mktime(&info);
    if( ret == -1 ) {
        printf("Error: unable to make time using mktime\n");
    } else {
        strftime(buffer, sizeof(buffer), "%c", &info );
        printf(buffer);
    }

    return 0;
}