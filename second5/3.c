//#include <bits/pthreadtypes.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

#define printf(format, ...) printf(format, ##__VA_ARGS__); fflush(stdout);

#define CNT 5
sem_t spoons[CNT];
pthread_mutex_t waitEveryone;

void* eatOrThink(void* arg) {
    int id = *(int*)arg;
    int leftSpoon = id;
    int rightSpoon = (id + 1) % CNT;

    while(1) {
        pthread_mutex_lock(&waitEveryone);
        // взятие правой и левой транзакция -> должно произойти одновременно
        sem_wait(&spoons[leftSpoon]);
        sem_wait(&spoons[rightSpoon]);

        pthread_mutex_unlock(&waitEveryone);

        printf("Философ%d 😋\n", id + 1);
        //sleep(1); // едим

        sem_post(&spoons[leftSpoon]);
        sem_post(&spoons[rightSpoon]);

        printf("Философ%d 🤔\n", id + 1);
        //sleep(1); // думаем
    }

    return NULL;
}

void* grabAndEat(void* arg) {
    int id = *(int*)arg;
    int leftSpoon = id;
    int rightSpoon = (id + 1) % CNT;

    while(1) {
        //pthread_mutex_lock(&waitEveryone);

        sem_wait(&spoons[leftSpoon]);
        sem_wait(&spoons[rightSpoon]);

        //pthread_mutex_unlock(&waitEveryone);

        printf("Философ%d 🥄😋🥄\n", id + 1);
        //sleep(1); // едим

        sem_post(&spoons[leftSpoon]);
        sem_post(&spoons[rightSpoon]);

        printf("Философ%d 🤔\n", id + 1);
        //sleep(1); // думаем
    }

    return NULL;
}

int main(int argc, char *argv[]) {

    for (int i = 0; i < CNT; ++i) {
        if(sem_init(&spoons[i], 0, 1) != 0) {
            return 1;
        }
    }
    if(pthread_mutex_init(&waitEveryone, NULL) != 0) {
        return 1;
    }

    pthread_t philosophers[CNT];
    int id[CNT];
    for (int i = 0; i < CNT; ++i) id[i] = i;

    for (int i = 0; i < CNT; ++i){
        // grabAndEat - проблема
        // eatOrThink - решение
        pthread_create(&philosophers[i], NULL, eatOrThink, &id[i]);
    }
    for (int i = 0; i < CNT; ++i){
        pthread_join(philosophers[i], NULL);
    }

    for (int i = 0; i < CNT; ++i) {
        if(sem_destroy(&spoons[i])) {
            return 1;
        }
    }
    pthread_mutex_destroy(&waitEveryone);
    return 0;
}


