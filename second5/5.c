//#include <bits/pthreadtypes.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define printf(format, ...) printf(format, ##__VA_ARGS__); fflush(stdout);

pthread_mutex_t mutexRoom = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t womanCanEnter = PTHREAD_COND_INITIALIZER;
pthread_cond_t manCanEnter = PTHREAD_COND_INITIALIZER;

int womanRoomCnt = 0;
int manRoomCnt = 0;

void woman_wants_to_enter(int roomSize) {
    pthread_mutex_lock(&mutexRoom);

    while (womanRoomCnt >= roomSize || manRoomCnt > 0) {
        // if не используем из-за случайного ложного пробуждения
        pthread_cond_wait(&womanCanEnter, &mutexRoom);
    }
    ++womanRoomCnt;

    pthread_mutex_unlock(&mutexRoom);
}

void man_wants_to_enter(int roomSize) {
    pthread_mutex_lock(&mutexRoom);

    while (manRoomCnt >= roomSize || womanRoomCnt > 0) {
        // if не используем из-за случайного ложного пробуждения
        pthread_cond_wait(&manCanEnter, &mutexRoom);
    }
    ++manRoomCnt;

    pthread_mutex_unlock(&mutexRoom);
}

void woman_leaves() {
    pthread_mutex_lock(&mutexRoom);

    --womanRoomCnt;
    if (womanRoomCnt > 0) {
        printf("+ ");
        pthread_cond_broadcast(&womanCanEnter);
    } else {
        printf("\nRoom: ");
        pthread_cond_broadcast(&manCanEnter);
        pthread_cond_broadcast(&womanCanEnter);
    }

    pthread_mutex_unlock(&mutexRoom);
}

void man_leaves() {
    pthread_mutex_lock(&mutexRoom);

    --manRoomCnt;
    if (manRoomCnt > 0) {
        printf("+ ");
        pthread_cond_broadcast(&manCanEnter);
    } else {
        printf("\nRoom: ");
        pthread_cond_broadcast(&womanCanEnter);
        pthread_cond_broadcast(&manCanEnter);
    }

    pthread_mutex_unlock(&mutexRoom);
}

void* womanShower(void* arg) {
    int roomSize = *(int *) arg;

    woman_wants_to_enter(roomSize);
    printf("W ");
    sleep(1);

    woman_leaves();

    return NULL;
}

void* manShower(void* arg) {
    int roomSize = *(int *) arg;

    man_wants_to_enter(roomSize);
    printf("M ");
    sleep(1);

    man_leaves();

    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2)
    {
        printf("Программа.exe N\n");
        return 0;
    }

    //const int N = atoi(argv[1]);
    char* endptr;
    const int N = strtol(argv[1], &endptr, 10);
    if (N <= 0 || (*endptr != '\0' && *endptr != '\n')) 
    {
        printf("Некорректный ввод\n");
        return 0;
    }

    int NAlias = N;
    const int menCnt = 2*N-3;
    const int womenCnt = 3*N-1;
    pthread_t men[menCnt];
    pthread_t women[womenCnt];
    printf("Room: ");

    for (int i = 0; i < menCnt; ++i){
        pthread_create(&men[i], NULL, manShower, &NAlias);
    }
    for (int i = 0; i < womenCnt; ++i){
        pthread_create(&women[i], NULL, womanShower, &NAlias);
    }
    for (int i = 0; i < menCnt; ++i){
        pthread_join(men[i], NULL);
    }
    for (int i = 0; i < womenCnt; ++i){
        pthread_join(women[i], NULL);
    }

    printf("\n");
    return 0;
}
