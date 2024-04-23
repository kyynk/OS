#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 4096

int main() {
    char sourceFileName[100];
    char destinationFileName[100];
    FILE *sourceFile, *destinationFile;
    size_t bytesRead;
    char buffer[BUFFER_SIZE];

    // Prompt user for file names
    printf("Source file: ");
    scanf("%s", sourceFileName);
    printf("Destination file: ");
    scanf("%s", destinationFileName);

    // Open source file for reading
    sourceFile = fopen(sourceFileName, "rb");
    if (sourceFile == NULL) {
        perror("Error opening source file");
        return EXIT_FAILURE;
    }

    // Open destination file for writing
    destinationFile = fopen(destinationFileName, "wb");
    if (destinationFile == NULL) {
        perror("Error opening destination file");
        fclose(sourceFile);
        return EXIT_FAILURE;
    }

    // Copy contents from source to destination
    while ((bytesRead = fread(buffer, 1, BUFFER_SIZE, sourceFile)) > 0) {
        fwrite(buffer, 1, bytesRead, destinationFile);
    }

    // Close files
    fclose(sourceFile);
    fclose(destinationFile);

    printf("File copied successfully.\n");

    return EXIT_SUCCESS;
}

