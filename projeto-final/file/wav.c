#include "stdio.h"
#include <stdlib.h>
#include "ezdsp5502.h"
#include "wav.h"

FILE *open_wav_file(const char *filename)
{

    FILE *fp = fopen(filename, "rb");
    if (fp == NULL)
    {
        printf("Failed to open input WAV file: %s\n", filename);
        exit(0);
    }
    return fp;
}

FILE *create_file(const char *filename)
{

    FILE *fp = fopen(filename, "wb");

    if (fp == NULL)
    {

        printf("Error: Failed to create the file '%s'.\n", filename);
        return NULL;
    }

    printf("File '%s' created successfully.\n", filename);

    return fp;
}
