#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define TOTAL_POINTS 10000000 // Total number of random points to generate
#define NUM_OF_THREAD 5 // Number of threads

int points_in_circle = 0; // Global variable to store the count of points within the circle
pthread_mutex_t mutex; // Mutex

void *monte_carlo(void *arg) {
    int i;
    int local_count = 0;

    for (i = 0; i < TOTAL_POINTS / NUM_OF_THREAD; i++) {
        double x = (double)rand() / RAND_MAX; // Random x coordinate between 0 and 1
        double y = (double)rand() / RAND_MAX; // Random y coordinate between 0 and 1

        // Check if the point (x, y) is inside the unit circle
        if ((x * x + y * y) <= 1) {
            local_count++;
        }
    }

    pthread_mutex_lock(&mutex);
    // Update the global count of points within the circle
    points_in_circle += local_count;
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

int main() {
    pthread_t tid[NUM_OF_THREAD];
    pthread_mutex_init(&mutex, NULL); // Initialize mutex
    srand(time(NULL)); // Seed the random number generator

    for (int i=0; i<NUM_OF_THREAD; i++) {
        // Create a thread to generate random points
        if (pthread_create(&tid[i], NULL, monte_carlo, NULL) != 0) {
            perror("pthread_create");
            return 1;
        }
    }

    for (int i=0; i<NUM_OF_THREAD; i++) {
        // Wait for the thread to finish
        if (pthread_join(tid[i], NULL) != 0) {
            perror("pthread_join");
            return 1;
        }
    }

    pthread_mutex_destroy(&mutex);
    // Estimate the value of pi using the generated points
    double pi = 4.0 * ((double)points_in_circle / TOTAL_POINTS);
    printf("Estimated value of pi: %f\n", pi);
    // printf("points_in_circle: %d\n", points_in_circle);

    return 0;
}
