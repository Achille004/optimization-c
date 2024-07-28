#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define MAXC 1073741824
#define BUFFER_SIZE 4096

typedef struct timeval timeval;

int checkConstants(const int LEN, const char *src);

// Avg execution time for 1 GB of data: ~35 sec (1 core @ 3.6 Ghz)
// .\substringFinder.exe "26" "input.txt" ["substringOutput.txt"]
int main(int argc, char** argv)
{
    if (argc < 3) {
        fprintf(stderr, "Not enough arguments\n");
        return EXIT_FAILURE;
    }

    const int LEN = atoi(argv[1]);

    if (!checkConstants(LEN, argv[1]))
        return EXIT_FAILURE;

    struct timeval t1, t2;
    gettimeofday(&t1, NULL);

    FILE *in = fopen(argv[2], "r"), *out = argc > 3 ? fopen(argv[3], "w") : stdout;
    if (!in || !out) {
        fprintf(stderr, "Error opening files.");
        return EXIT_FAILURE;
    }

    char buffer[BUFFER_SIZE], *bufferPos;
    size_t bufferLen;
    for (int i = 0; i < (MAXC - LEN); i += 1) {
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

        int currentAlphaBits = 0;
        int ok = 1;

        for (int j = 0; j < LEN && ok; j += 1) {
            // Works only for lowercase characters
            int alphaBit = 1 << (bufferPos[j] - 'a');

            if (!(currentAlphaBits & alphaBit))
                currentAlphaBits |= alphaBit;
            else
                ok = 0;
        }

        if (ok) {
            fwrite(bufferPos, 1, LEN, out);
            fprintf(out, " (index: %d)\n", i);
        }

        bufferPos++;
    }

    fclose(in);
    if(argc > 3) fclose(out);
    gettimeofday(&t2, NULL);

    printf("Execution time: %.3lf ms\n", (double)(t2.tv_usec - t1.tv_usec) / 1000 + (double)(t2.tv_sec - t1.tv_sec) * 1000);

    return 0;
}

int checkConstants(const int LEN, const char *src)
{
    char temp[3];
    itoa(LEN, temp, 10);
    temp[2] = '\0';

    if (strcmp(src, temp)) {
        fprintf(stderr, "Error: LEN is not an integer.\n");
        return 0;
    }

    if (LEN <= 0 || LEN > 26) {
        fprintf(stderr, "Error: LEN must be between 1 and 26.\n");
        return 0;
    }

    if (MAXC <= LEN) {
        fprintf(stderr, "Error: MAXC must be greater than or equal to LEN.\n");
        return 0;
    }

    if (BUFFER_SIZE <= LEN) {
        fprintf(stderr, "Error: BUFFER_SIZE must be greater or equal to LEN.\n");
        return 0;
    }

    return 1;
}