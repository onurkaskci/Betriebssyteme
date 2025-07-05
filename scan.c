#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define MAX_REQUESTS 10
#define UP 1
#define DOWN -1

#define CHK(PTR, ERR, EX) ({if (!(PTR)) {fprintf(stderr, "\033[31m\033[1mError:\033[0m %s\n\r", ERR); exit(EX);}})
#define CHK1(PTR) CHK(PTR, "Memory allocation failed", 125)
#define USAGEINFO(ARGV) printf("Usage: %s Lx = {..., ...}, ...\nExample: ", ARGV[0])

typedef struct {
    int arrivaltime;
    int count;
    unsigned int requests[MAX_REQUESTS];
} RequestBatch;

RequestBatch example[] = {
    {0, 4, {5, 15, 2, 9}},
    {2, 3, {4, 10, 1}},
    {6, 3, {8, 6, 14}},
    {10, 1, {0}}
};

void add_requests(unsigned int* queue, int* queue_size, unsigned int* new_requests, int count) {
    for (int i = 0; i < count; i++) queue[(*queue_size)++] = new_requests[i];
}

void remove_request(unsigned int* queue, int* queue_size, int index) {
    for (int j = index; j < (*queue_size) - 1; j++) queue[j] = queue[j + 1];
    (*queue_size)--;
}

int compare_up(const void *a, const void *b) { return (*(unsigned int *)a - *(unsigned int *)b); }

int compare_down(const void *a, const void *b) { return (*(unsigned int *)b - *(unsigned int *)a); }


int SCAN(int current_track, int* direction, unsigned int* queue, int* queue_size, unsigned int duration) {
    unsigned int count = 0, *candidates = malloc(duration * MAX_REQUESTS * sizeof(unsigned int)); CHK1(candidates);
    
    while (true) {
        if (*queue_size == 0) { free(candidates); return current_track; }
        else { count = 0; }
        
        ///TODO1
        
        // Change direction and continue if count = 0
        ///TODO2
        
        // Sort candidates with qsort
        ///TODO3

        // Set current_track to next candidate
        ///TODO4
        
        ///TODO5
        
        free(candidates);
        return current_track;
    }
}

void compact(char* str) {
    char* p = str, *q = str;
    while (*q) if (!isspace(*q)) *p++ = *q++;
    *p = '\0';
}

int parse_batches(const char* arg, RequestBatch* out, int max_batches) {
    int count = 0;
    char* input = strdup(arg);
    CHK1(input);
    compact(input);

    char* ptr = input;
    while (*ptr && count < max_batches) {
        if (*ptr == 'L' || *ptr == 'l') {
            ptr++;
            if (!isdigit(*ptr)) { count = 0; break; }
            int time = atoi(ptr);

            while (*ptr && *ptr != '{') ptr++;
            if (*ptr != '{') { count = 0; break; }

            char* brace = ptr;
            char* end = strchr(brace, '}');
            if (!end) { count = 0; break; }
            *end = '\0';

            RequestBatch* b = &out[count];
            b->arrivaltime = time;
            b->count = 0;

            char* num = strtok(brace + 1, ",");
            while (num && b->count < MAX_REQUESTS) {
                if (!isdigit(num[0])) { count = 0; break; }
                b->requests[b->count++] = (unsigned int)atoi(num);
                num = strtok(NULL, ",");
            }

            if (b->count == 0) { count = 0; break; }

            count++;
            ptr = end + 1;
        } else {
            ptr++;
        }
    }

    free(input);
    return count;
}

void print_batch(RequestBatch* b, int n) {
    for (int i = 0; i < n; i++) {
        printf("L%d = {", b[i].arrivaltime);
        for (int j = 0; j < b[i].count; j++) {
            printf("%u%s", b[i].requests[j], (j < b[i].count - 1) ? ", " : "");
        }
        printf("}%s", (i < n - 1) ? ", " : "\n");
    }
}

int main(int argc, char* argv[]) {
    RequestBatch* batches = NULL;
    int num_batches = 0;
    unsigned int end_of_simulation = 0;

    if (argc < 2) {
        USAGEINFO(argv);
        print_batch(example, sizeof(example) / sizeof(example[0]));
        batches = example;
        num_batches = sizeof(example) / sizeof(example[0]);
        for (int i = 0; i < num_batches; i++)
            if ((unsigned)batches[i].arrivaltime > end_of_simulation)
                end_of_simulation = batches[i].arrivaltime;
    } else {
        char argstr[4096] = {0};
        for (int i = 1; i < argc; i++) strcat(argstr, argv[i]);
        batches = malloc(sizeof(RequestBatch) * 128);
        CHK1(batches);
        num_batches = parse_batches(argstr, batches, 128);
        if (num_batches == 0) {
            USAGEINFO(argv);
            print_batch(example, sizeof(example) / sizeof(example[0]));
            batches = example;
            num_batches = sizeof(example) / sizeof(example[0]);
        }
        for (int i = 0; i < num_batches; i++)
            if ((unsigned)batches[i].arrivaltime > end_of_simulation)
                end_of_simulation = batches[i].arrivaltime;
    }

    end_of_simulation += 5;
    int direction = UP, queue_size = 0, current_track = 0;
    unsigned int* queue = malloc(end_of_simulation * MAX_REQUESTS * sizeof(unsigned int));
    CHK1(queue);

    printf("Tracks: ");
    for (unsigned int t = 0; t <= end_of_simulation || queue_size > 0; t++) {
        for (int i = 0; i < num_batches; i++)
            if ((unsigned)batches[i].arrivaltime == t)
                add_requests(queue, &queue_size, batches[i].requests, batches[i].count);
        if (queue_size > 0) {
            int prev = current_track;
            current_track = SCAN(current_track, &direction, queue, &queue_size, end_of_simulation);
            if (current_track != prev)
                printf("%u, ", current_track);
        }
    }

    printf("\b\b \n\r");
    free(queue);
    if (argc > 1) free(batches);
    return 0;
}