#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define MAXC 1073741824
#define BUFFER_SIZE 4096

typedef struct timeval timeval;

int checkConstants(const int LEN);

// Avg execution time for 1 GB of data: ~15 sec (1 core @ 3.6 Ghz)
// .\wordFinder.exe "int" "input.txt" ["wordOutput.txt"]
int main(int argc, char** argv)
{
    if (argc < 3) {
        fprintf(stderr, "Not enough arguments\n");
        return EXIT_FAILURE;
    }

    const char* WORD = argv[1];
    const int LEN = strlen(WORD);

    if (!checkConstants(LEN)) {
        fprintf(stderr, "Invalid constants configuration.\n");
        return EXIT_FAILURE;
    }

    struct timeval t1, t2;
    gettimeofday(&t1, NULL);

    FILE *in = fopen(argv[2], "r"), *out = argc > 3 ? fopen(argv[3], "w") : stdout;
    if (!in || !out) {
        fprintf(stderr, "Error opening files.");
        return EXIT_FAILURE;
    }

    char buffer[BUFFER_SIZE], temp[LEN + 1], *bufferPos;
    int current, ok;
    size_t bufferLen;
    temp[LEN] = '\0';
    for (int i = 0; i < (MAXC - LEN); i++) {
        if (buffer + bufferLen - LEN + 1 == bufferPos || i == 0) {
            if (i != 0) {
                memcpy(buffer, bufferPos, LEN - 1);
                bufferLen = fread(buffer + LEN - 1, sizeof(char), sizeof(buffer) - LEN + 1, in);
            } else {
                bufferLen = fread(buffer, sizeof(char), sizeof(buffer), in);
            }

            if (!bufferLen)
                break;

            bufferPos = buffer;
        }

        current = 0;
        ok = 1;

        memcpy(temp, bufferPos, LEN);
        if (!strcmp(temp, WORD)) {
            fprintf(out, "%d\n", i);
        }

        bufferPos++;
    }

    fclose(in);
    if(argc > 3) fclose(out);
    gettimeofday(&t2, NULL);

    printf("Execution time: %.3lf ms\n", (double)(t2.tv_usec - t1.tv_usec) / 1000 + (double)(t2.tv_sec - t1.tv_sec) * 1000);
    return 0;
}

// Function to check validity of constants
int checkConstants(const int LEN)
{
    // Ensure BUFFER_SIZE is reasonable for memory usage
    if (BUFFER_SIZE <= LEN) {
        fprintf(stderr, "Error: MAXC must be greater than or equal to LEN.\n");
        return 0;
    }

    // Ensure MAXC is large enough to contain at least one set of LEN characters
    if (MAXC < LEN) {
        fprintf(stderr, "Error: BUFFER_SIZE must be greater or equal to LEN.\n");
        return 0;
    }

    return 1;
}