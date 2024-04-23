#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source-file> <destination-file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *source_file = argv[1];
    char *destination_file = argv[2];

    // Create pipe
    int pipe_fd[2];
    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Fork process
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // Child process
        // Close writing end of pipe
        close(pipe_fd[1]);

        // Open destination file for writing
        int dest_fd = open(destination_file, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        if (dest_fd == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }

        // Read from pipe and write to destination file
        char buffer[BUFFER_SIZE];
        ssize_t bytes_read;
        while ((bytes_read = read(pipe_fd[0], buffer, BUFFER_SIZE)) > 0) {
            if (write(dest_fd, buffer, bytes_read) != bytes_read) {
                perror("write");
                exit(EXIT_FAILURE);
            }
        }
        
        if (bytes_read == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        // Close file descriptors
        close(pipe_fd[0]);
        close(dest_fd);
        exit(EXIT_SUCCESS);
    } else { // Parent process
        // Close reading end of pipe
        close(pipe_fd[0]);

        // Open source file for reading
        int source_fd = open(source_file, O_RDONLY);
        if (source_fd == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }

        // Read from source file and write to pipe
        char buffer[BUFFER_SIZE];
        ssize_t bytes_read;
        while ((bytes_read = read(source_fd, buffer, BUFFER_SIZE)) > 0) {
            if (write(pipe_fd[1], buffer, bytes_read) != bytes_read) {
                perror("write");
                exit(EXIT_FAILURE);
            }
        }

        if (bytes_read == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        // Close file descriptors
        close(pipe_fd[1]);
        close(source_fd);
        exit(EXIT_SUCCESS);
    }

    return 0;
}

