#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_SIZE 100

// Structure to pass arguments to the thread function
struct ThreadArgs {
    int n; // Number of Fibonacci numbers to generate
    int fib[MAX_SIZE]; // Array to store Fibonacci sequence
};
pthread_mutex_t mutex; // Mutex

// Function to generate Fibonacci sequence
void *generate_fibonacci(void *args) {
    struct ThreadArgs *thread_args = (struct ThreadArgs *)args;
    int n = thread_args->n;
    int *fib = thread_args->fib;

    fib[0] = 0;
    fib[1] = 1;
    
    for (int i = 2; i < n; i++) {
        fib[i] = fib[i-1] + fib[i-2];
    }

    pthread_mutex_unlock(&mutex);
}

int main() {
    int n;

    printf("Enter the number of Fibonacci numbers to generate: ");
    scanf("%d", &n);

    if (n <= 0 || n > MAX_SIZE) {
        printf("Invalid input.\nNumber should be in range (0, 100].");
        return 1;
    }

    // Declare thread ID and arguments
    pthread_t tid;
    struct ThreadArgs args;
    args.n = n;
    pthread_mutex_init(&mutex, NULL); // Initialize mutex

    pthread_mutex_lock(&mutex);
    // Create thread
    pthread_create(&tid, NULL, generate_fibonacci, (void *)&args);
    
    pthread_mutex_lock(&mutex);
    printf("Fibonacci sequence:\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", args.fib[i]);
    }
    printf("\n");
    pthread_mutex_unlock(&mutex);

    // Destroy mutex
    pthread_mutex_destroy(&mutex);

    return 0;
}
