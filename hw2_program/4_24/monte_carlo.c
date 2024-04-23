#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define TOTAL_POINTS 10000000 // Total number of random points to generate

int points_in_circle = 0; // Global variable to store the count of points within the circle

void *monte_carlo(void *arg) {
    int i;
    int local_count = 0;

    for (i = 0; i < TOTAL_POINTS; i++) {
        double x = (double)rand() / RAND_MAX; // Random x coordinate between 0 and 1
        double y = (double)rand() / RAND_MAX; // Random y coordinate between 0 and 1

        // Check if the point (x, y) is inside the unit circle
        if ((x * x + y * y) <= 1) {
            local_count++;
        }
    }

    // Update the global count of points within the circle
    points_in_circle += local_count;

    pthread_exit(NULL);
}

int main() {
    pthread_t tid;
    srand(time(NULL)); // Seed the random number generator

    // Create a thread to generate random points
    if (pthread_create(&tid, NULL, monte_carlo, NULL) != 0) {
        perror("pthread_create");
        return 1;
    }

    // Wait for the thread to finish
    if (pthread_join(tid, NULL) != 0) {
        perror("pthread_join");
        return 1;
    }

    // Estimate the value of pi using the generated points
    double pi = 4.0 * ((double)points_in_circle / TOTAL_POINTS);
    printf("Estimated value of pi: %f\n", pi);

    return 0;
}
