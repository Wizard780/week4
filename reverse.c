#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "wav.h"

int check_format(WAVHEADER header);
int get_block_size(WAVHEADER header);

int main(int argc, char *argv[])
{
    // Ensure proper usage
    if (argc != 3)
    {
        printf("Usage: ./reverse input.wav output.wav\n");
        return 1;
    }

    // Open input file for reading
    FILE *inptr = fopen(argv[1], "r");
    if (inptr == NULL)
    {
        printf("Could not open file\n");
        return 2;
    }

    // Read header
    WAVHEADER header;
    fread(&header, sizeof(WAVHEADER), 1, inptr);

    // Use check_format to ensure WAV format
    if (check_format(header) == 0)
    {
        printf("Not a wav file\n");
    }

    // Open output file for writing
    FILE *output = fopen(argv[2], "w");

    // Write header to file
    fwrite(&header, sizeof(WAVHEADER), 1, output);

    // Use get_block_size to calculate size of block
    int block_size = get_block_size(header);

    // Write reversed audio to file
    BYTE buffer[block_size];
    fseek(inptr, 0, SEEK_END);
    long audio_size = ftell(inptr) - sizeof(WAVHEADER);
    int audio_block = (int) audio_size / block_size;
    for (int i = audio_block - 1; i >= 0; i--)
    {
        fseek(inptr, sizeof(WAVHEADER) + i * block_size, SEEK_SET);
        fread(buffer, block_size, 1, inptr);
        fwrite(buffer, block_size, 1, output);
    }
    fclose(inptr);
}

int check_format(WAVHEADER header)
{
    if (header.format[0] == 'W' && header.format[1] == 'A' && header.format[2] == 'V' && header.format[3] == 'E')
    {
        return 1;
    }
    return 0;
}

int get_block_size(WAVHEADER header)
{
    WORD bytePerSample = header.bitsPerSample / 8;
    return (header.numChannels * bytePerSample);
    return 0;
}
