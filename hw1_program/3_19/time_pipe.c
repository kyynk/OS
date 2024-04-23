#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <command>\n", argv[0]);
        return 1;
    }

    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return 1;
    }

    struct timeval start_time, end_time;
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    } else if (pid == 0) { // Child process
        close(pipefd[0]); // Close reading end
        gettimeofday(&start_time, NULL);
        write(pipefd[1], &start_time, sizeof(struct timeval));
        close(pipefd[1]);
        execvp(argv[1], &argv[1]);
        perror("execvp");
        exit(1);
    } else { // Parent process
        wait(NULL);
        close(pipefd[1]); // Close writing end
        read(pipefd[0], &start_time, sizeof(struct timeval));
        gettimeofday(&end_time, NULL);
        close(pipefd[0]);
        long elapsed_sec = end_time.tv_sec - start_time.tv_sec;
        long elapsed_usec = end_time.tv_usec - start_time.tv_usec;
        printf("Elapsed time: %ld.%06ld seconds\n", elapsed_sec, elapsed_usec);
    }

    return 0;
}
