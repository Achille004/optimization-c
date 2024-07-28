#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 1073741824
#define LOWER 'a'
#define UPPER 'z'
#define OUTPUT "input.txt"

int randInt(int lower, int upper)
{
    return rand() % (upper - lower + 1) + lower;
}

int main()
{
    time_t t;
    srand((unsigned)time(&t));

    FILE *out = fopen(OUTPUT, "w");
    for (int i = 0; i < N; i++)
    {
        fputc(randInt(LOWER, UPPER), out);
    }
    fclose(out);
}