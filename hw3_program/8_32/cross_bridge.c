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
    sleep(rand() % 3); // Simulate travel time
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
    sleep(rand() % 3); // Simulate travel time
    printf("Southbound farmer is crossing done.\n");

    pthread_mutex_lock(&mutex);
    southbound--;
    pthread_cond_signal(&bridge);
    pthread_mutex_unlock(&mutex);
}

int main() {
    int n;
    printf("input even number of total farmers: ");
    scanf("%d", &n);
    if (n > 10 || n <= 0 || n%2 == 1) {
        printf("please input a even number which >0, <=10\n");
        return 1;
    }
    pthread_t threads[n];
    srand(time(NULL));

    for (int i = 0; i < n/2; i++) {
        pthread_create(&threads[i], NULL, northbound_farmer, NULL);
        pthread_create(&threads[i+n/2], NULL, southbound_farmer, NULL);
    }

    for (int i = 0; i < n; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
