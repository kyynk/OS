#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <fcntl.h>

#define SHM_NAME "/time_shm"
#define SHM_SIZE sizeof(struct timeval)

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <command>\n", argv[0]);
        return 1;
    }

    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        return 1;
    }
    ftruncate(shm_fd, SHM_SIZE);

    struct timeval *start_time = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (start_time == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    } else if (pid == 0) { // Child process
        gettimeofday(start_time, NULL);
        execvp(argv[1], &argv[1]);
        perror("execvp");
        exit(1);
    } else { // Parent process
        wait(NULL);
        struct timeval end_time;
        gettimeofday(&end_time, NULL);
        long elapsed_sec = end_time.tv_sec - start_time->tv_sec;
        long elapsed_usec = end_time.tv_usec - start_time->tv_usec;
        printf("Elapsed time: %ld.%06ld seconds\n", elapsed_sec, elapsed_usec);
    }

    shm_unlink(SHM_NAME);
    return 0;
}
