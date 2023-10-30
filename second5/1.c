#include <stdio.h>
#include <unistd.h>

#define printf(format, ...) printf(format, ##__VA_ARGS__); fflush(stdout);

int main(int argc, char *argv[]) {
    
    pid_t pid = getpid();
    pid_t ppid = getppid();
    pid_t pgrp = getpgrp();
    uid_t uid = getuid(); // Реальный UID отражает, от имени какого пользователя выполняется процесс.
    gid_t gid = getgid();
    uid_t euid = geteuid(); // Эффективный UID определяет привилегии и уровень доступа в данный момент.
    gid_t egid = getegid();

    printf("PID: %d\n", pid);
    printf("PPID: %d\n", ppid);
    printf("PGRP: %d\n", pgrp);
    printf("UID: %d\n", uid);
    printf("GID: %d\n", gid);
    printf("EUID: %d\n", euid);
    printf("EGID: %d\n", egid);

    return 0;
}


