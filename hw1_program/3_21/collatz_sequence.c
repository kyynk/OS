#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

// Function to generate the Collatz sequence
void collatz_sequence(int n) {
    while (n != 1) {
        printf("%d ", n);
        if (n % 2 == 0)
            n = n / 2;
        else
            n = 3 * n + 1;
    }
    printf("1\n");
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <positive_integer>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    if (n <= 0) {
        printf("Error: Please provide a positive integer.\n");
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0) {
        printf("Fork failed\n");
        return 1;
    } else if (pid == 0) {  // Child process
        // printf("Child process with PID %d\n", getpid());
        collatz_sequence(n);
    } else {  // Parent process
        wait(NULL);
        // printf("Parent process with PID %d waited for child with PID %d\n", getpid(), pid);
    }

    return 0;
}

