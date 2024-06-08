#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define CYLINDERS 5000
#define REQUESTS 1000

void generate_requests(int *requests, int length) {
    for (int i = 0; i < length; i++) {
        requests[i] = rand() % CYLINDERS;
    }
}

void print_requests(int *requests, int length) {
    printf("Cylinder Requests: ");
    for (int i = 0; i < length; i++) {
        printf("%d ", requests[i]);
    }
    printf("\n");
}

int cmpfunc(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

int FCFS(int requests[], int length, int head_track) {
    int total_movement = 0;
    int current_track = head_track;

    for (int i = 0; i < length; i++) {
        total_movement += abs(requests[i] - current_track);
        current_track = requests[i];
    }

    return total_movement;
}

int SCAN(int requests[], int length, int head_track) {
    int total_movement = 0;
    int current_track = head_track;
    int direction = 1; // 1 for up, -1 for down

    int sorted[REQUESTS];
    for (int i = 0; i < length; i++) {
        sorted[i] = requests[i];
    }
    qsort(sorted, length, sizeof(int), cmpfunc);

    int index;
    for (index = 0; index < length; index++) {
        if (sorted[index] > head_track) {
            break;
        }
    }

    // method 1
    // if index > 0, this mean head track is not the smallest
    // then we can know total movement should be: (4999 - head track) + abs(the smallest track - 4999)
    // if index == 0, this mean head track is the smallest
    // then we can know total movement should be: the largest track - head track

    if (index > 0) {
        printf("SCAN Total Movement (method 1): %d\n", ((CYLINDERS - 1) - head_track) + abs(sorted[0] - (CYLINDERS - 1)));
    }
    else {
        printf("SCAN Total Movement (method 1): %d\n", (sorted[length - 1] - head_track));
    }

    // method 2
    for (int i = index; i < length; i++) {
        total_movement += abs(sorted[i] - current_track);
        current_track = sorted[i];
    }
    if (index > 0) {
        total_movement += abs(current_track - (CYLINDERS - 1));
        current_track = CYLINDERS - 1;
        for (int i = index - 1; i >= 0; i--) {
            total_movement += abs(sorted[i] - current_track);
            current_track = sorted[i];
        }
    }

    return total_movement;
}

int C_SCAN(int requests[], int length, int head_track) {
    int total_movement = 0;
    int current_track = head_track;

    int sorted[REQUESTS];
    for (int i = 0; i < length; i++) {
        sorted[i] = requests[i];
    }
    qsort(sorted, length, sizeof(int), cmpfunc);

    int index;
    for (index = 0; index < length; index++) {
        if (sorted[index] > head_track) {
            break;
        }
    }

    // method 1
    // if index > 0, this mean head track is not the smallest
    // then we can know total movement should be: (4999 - head track) + (sorted[index - 1] - 0)
    // if index == 0, this mean head track is the smallest
    // then we can know total movement should be: the largest track - head track

    if (index > 0) {
        printf("C-SCAN Total Movement (method 1): %d\n", ((CYLINDERS - 1) - head_track) + (sorted[index - 1] - 0));
    }
    else {
        printf("C-SCAN Total Movement (method 1)a: %d\n", (sorted[length - 1] - head_track));
    }

    // method 2
    for (int i = index; i < length; i++) {
        total_movement += abs(sorted[i] - current_track);
        current_track = sorted[i];
    }
    if (index > 0) {
        total_movement += abs(current_track - (CYLINDERS - 1));
        current_track = 0;

        for (int i = 0; i < index; i++) {
            total_movement += abs(sorted[i] - current_track);
            current_track = sorted[i];
        }
    }

    return total_movement;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <initial head track>\n", argv[0]);
        return 1;
    }

    int head_track = atoi(argv[1]);
    if (head_track < 0 || head_track >= CYLINDERS) {
        printf("Initial head track must be between 0 and %d\n", CYLINDERS - 1);
        return 1;
    }

    int requests[REQUESTS];

    srand(time(NULL));
    generate_requests(requests, REQUESTS);
    print_requests(requests, REQUESTS);

    printf("\nFCFS Total Movement: %d\n", FCFS(requests, REQUESTS, head_track));
    printf("SCAN Total Movement (method 2): %d\n", SCAN(requests, REQUESTS, head_track));
    printf("C-SCAN Total Movement (method 2): %d\n", C_SCAN(requests, REQUESTS, head_track));

    return 0;
}
