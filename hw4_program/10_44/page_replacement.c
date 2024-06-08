#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_FRAMES 10
#define PAGE_REFERENCES 20
#define PAGE_RANGE 10

void generate_page_references(int *references, int length) {
    for (int i = 0; i < length; i++) {
        references[i] = rand() % PAGE_RANGE;
    }
}

void print_page_references(int *references, int length) {
    printf("Page Reference String: ");
    for (int i = 0; i < length; i++) {
        printf("%d ", references[i]);
    }
    printf("\n");
}

int FIFO(int frames[], int frame_count, int references[], int length) {
    int page_faults = 0;
    int index = 0;

    for (int i = 0; i < length; i++) {
        int page = references[i];
        int found = 0;

        for (int j = 0; j < frame_count; j++) {
            if (frames[j] == page) {
                found = 1;
                break;
            }
        }

        if (!found) {
            frames[index] = page;
            index = (index + 1) % frame_count;
            page_faults++;
        }
        printf("page:%d\t", page);
        printf("frame:");
        for (int j = 0; j < frame_count; j++) {
            printf(" %d", frames[j]);
        }
        printf("\tpage fault:%d\n", page_faults);
    }

    return page_faults;
}

int LRU(int frames[], int frame_count, int references[], int length) {
    int page_faults = 0;
    int counter[MAX_FRAMES] = {0};
    int time = 0;

    for (int i = 0; i < length; i++) {
        int page = references[i];
        int found = 0;

        for (int j = 0; j < frame_count; j++) {
            if (frames[j] == page) {
                found = 1;
                time++;
                counter[j] = time;
                break;
            }
        }

        if (!found) {
            int min = 0;
            for (int j = 1; j < frame_count; j++) {
                if (counter[j] < counter[min]) {
                    min = j;
                }
            }
            frames[min] = page;
            time++;
            counter[min] = time;
            page_faults++;
        }
        printf("page:%d\t", page);
        printf("frame:");
        for (int j = 0; j < frame_count; j++) {
            printf(" %d", frames[j]);
        }
        printf("\tpage fault:%d\n", page_faults);
    }

    return page_faults;
}

int OPT(int frames[], int frame_count, int references[], int length) {
    int page_faults = 0;
    int index = 0; // earlier index

    for (int i = 0; i < length; i++) {
        int page = references[i];
        int found = 0;

        for (int j = 0; j < frame_count; j++) {
            if (frames[j] == page) {
                found = 1;
                break;
            }
        }

        if (!found) {
            int farthest = i + 1;
            int replace_index = -1;

            for (int j = 0; j < frame_count; j++) {
                int k;
                for (k = i + 1; k < length; k++) {
                    if (frames[j] == references[k]) {
                        if (k > farthest) {
                            farthest = k;
                            replace_index = j;
                        }
                        break;
                    }
                }
                if (k == length) {
                    replace_index = j;
                    break;
                }
            }

            if (replace_index == -1) {
                replace_index = index;
            }

            frames[replace_index] = page;
            index = (index + 1) % frame_count;
            page_faults++;
        }
        printf("page:%d\t", page);
        printf("frame:");
        for (int j = 0; j < frame_count; j++) {
            printf(" %d", frames[j]);
        }
        printf("\tpage fault:%d\n", page_faults);
    }

    return page_faults;
}

void initialize_frames(int *frames, int frame_count) {
    for (int i = 0; i < frame_count; i++) {
        frames[i] = -1;
    }
    printf("frames:");
    for (int i = 0; i < frame_count; i++) {
        printf(" %d", frames[i]);
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <number of frames>\n", argv[0]);
        return 1;
    }

    int frame_count = atoi(argv[1]);
    if (frame_count <= 0 || frame_count > MAX_FRAMES) {
        printf("Number of frames must be between 1 and %d\n", MAX_FRAMES);
        return 1;
    }

    // int references[PAGE_REFERENCES] = {3, 1, 4, 2, 5, 4, 1, 3, 5, 2, 0, 1, 1, 0, 2, 3, 4, 5, 0, 1};
    int references[PAGE_REFERENCES];
    int frames[MAX_FRAMES];

    srand(time(NULL));
    generate_page_references(references, PAGE_REFERENCES);
    print_page_references(references, PAGE_REFERENCES);
    printf("=====FIFO=============================\n");
    initialize_frames(frames, frame_count);
    int FIFO_page_faults = FIFO(frames, frame_count, references, PAGE_REFERENCES);
    printf("=====LRU=============================\n");
    initialize_frames(frames, frame_count);
    int LRU_page_faults = LRU(frames, frame_count, references, PAGE_REFERENCES);
    printf("=====OPT=============================\n");
    initialize_frames(frames, frame_count);
    int OPT_page_faults = OPT(frames, frame_count, references, PAGE_REFERENCES);

    printf("\nFIFO Page Faults: %d\n", FIFO_page_faults);
    printf("LRU Page Faults: %d\n", LRU_page_faults);
    printf("OPT Page Faults: %d\n", OPT_page_faults);

    return 0;
}
