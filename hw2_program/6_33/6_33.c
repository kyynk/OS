#include <pthread.h>

#define MAX_RESOURCES 5
int available_resources = MAX_RESOURCES;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int decrease_count(int count) {
    pthread_mutex_lock(&mutex); // Acquire lock
    if (available_resources < count) {
        pthread_mutex_unlock(&mutex); // Release lock
        return -1;
    } else {
        available_resources -= count;
        pthread_mutex_unlock(&mutex); // Release lock
        return 0;
    }
}

int increase_count(int count) {
    pthread_mutex_lock(&mutex); // Acquire lock
    available_resources += count;
    pthread_mutex_unlock(&mutex); // Release lock
    return 0;
}
