#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t bridge = PTHREAD_COND_INITIALIZER;

int northbound = 0;
int southbound = 0;

void *northbound_farmer(void *arg) {
    pthread_mutex_lock(&mutex);
    while (southbound > 0) {
        pthread_cond_wait(&bridge, &mutex);
    }
    northbound++;
    pthread_mutex_unlock(&mutex);

    // Cross the bridge
    printf("Northbound farmer is crossing the bridge.\n");
    sleep(rand() % 5); // Simulate travel time
    printf("Northbound farmer is crossing done.\n");

    pthread_mutex_lock(&mutex);
    northbound--;
    pthread_cond_signal(&bridge);
    pthread_mutex_unlock(&mutex);
}

void *southbound_farmer(void *arg) {
    pthread_mutex_lock(&mutex);
    while (northbound > 0) {
        pthread_cond_wait(&bridge, &mutex);
    }
    southbound++;
    pthread_mutex_unlock(&mutex);

    // Cross the bridge
    printf("Southbound farmer is crossing the bridge.\n");
    sleep(rand() % 5); // Simulate travel time
    printf("Southbound farmer is crossing done.\n");

    pthread_mutex_lock(&mutex);
    southbound--;
    pthread_cond_signal(&bridge);
    pthread_mutex_unlock(&mutex);
}

int main() {
    pthread_t threads[6];
    srand(time(NULL));

    for (int i = 0; i < 6; i++) {
        if (rand()%2) {
            pthread_create(&threads[i], NULL, northbound_farmer, NULL);
        }
        else {
            pthread_create(&threads[i], NULL, southbound_farmer, NULL);
        }
    }

    for (int i = 0; i < 6; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
