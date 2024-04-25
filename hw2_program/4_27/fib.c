#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_SIZE 100

// Structure to pass arguments to the thread function
struct ThreadArgs {
    int n; // Number of Fibonacci numbers to generate
    int fib[MAX_SIZE]; // Array to store Fibonacci sequence
};

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
    
    pthread_exit(NULL);
}

int main() {
    int n;

    // Input the number of Fibonacci numbers to generate
    printf("Enter the number of Fibonacci numbers to generate: ");
    scanf("%d", &n);

    // Check input validity
    if (n <= 0 || n > MAX_SIZE) {
        printf("Invalid input.\nNumber should in range (0, 100].");
        return 1;
    }

    // Declare thread ID and arguments
    pthread_t tid;
    struct ThreadArgs args;
    args.n = n;

    // Create thread
    pthread_create(&tid, NULL, generate_fibonacci, (void *)&args);

    // Wait for thread to finish
    pthread_join(tid, NULL);

    // Output Fibonacci sequence
    printf("Fibonacci sequence:\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", args.fib[i]);
    }
    printf("\n");

    return 0;
}
