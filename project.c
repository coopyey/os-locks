#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include "project.h"

pthread_mutex_t mutex;
sem_t full, empty;
buffer_item buffer[SIZE];

int counter;
pthread_t tid;
pthread_attr_t attr;

int main() {
    int i, numProducer, numConsumer;
    int mainSleep = rand() % 4 + 1; //Time in s for main to sleep 1-5

    printf("Number of producers: ");
    scanf("%d", &numProducer);

    printf("Number of consumers: ");
    scanf("%d", &numConsumer);

    initData();

    // Create producer threads
    for (i = 0; i < numProducer; i++) {
        pthread_create(&tid, &attr, producer, NULL);
        printf("Producer thread created: %d\n", i);
    }

    // Create consumer threads
    for (i = 0; i < numConsumer; i++) {
        pthread_create(&tid, &attr, consumer, NULL);
        printf("Consumer thread created: %d\n", i);
    }

    sleep(mainSleep);
    exit(0);
}

void initData() {
    pthread_mutex_init(&mutex, NULL);
    sem_init(&full, 0, 0);
    sem_init(&empty, 0, SIZE);
    pthread_attr_init(&attr);
    counter = 0;
}

void *producer(void *param) {
    buffer_item item;

    for(;;) {
        int r_num = rand() % 7 + 1;
        sleep(r_num);

        item = rand();

        sem_wait(&empty);
        pthread_mutex_lock(&mutex);

        if (insert_item(item)) {
            fprintf(stderr, "Producer reported error.\n");
        } else {
            printf("Producer produced %d.\n", item);
        }

        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }
}

void *consumer(void *param) {
    buffer_item item;

    for(;;) {
        int r_num = rand() % 7 + 1;
        sleep(r_num);

        sem_wait(&full);
        pthread_mutex_lock(&mutex);

        if (remove_item(&item)) {
            fprintf(stderr, "Consumer reported error.\n");
        } else {
            printf("Consumer consumed %d.\n", item);
        }

        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    }
}

int insert_item(buffer_item item) {
    if (counter < SIZE) {
        buffer[counter] = item;
        counter++;
        return 0;
    } else { return -1; }
}

int remove_item(buffer_item *item) {
    if (counter > 0) {
        *item = buffer[(counter-1)];
        counter--;
        return 0;
    } else {return -1; }
}
