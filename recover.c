#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }
    else
    {
        // Open card.raw
        char *inputname = argv[1];
        FILE *inputp = fopen(inputname, "r");

        if (inputp == NULL)
        {
            printf("Error: cannot open %s\n", inputname);
            return 2;
        }
        BYTE buffer[512];
        int count = 0;
        FILE *imagep = NULL;
        char filename[8];
        // Repeat until end of card:
        while (fread(&buffer, 512, 1, inputp) == 1)
        {
            // If start of a new JPEG (0xff 0xd8 0xff 0xe*):
            if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
            {
                // If not first JPEG, close previous
                if (!(count == 0))
                {
                    fclose(imagep);
                }
                // Initialise file
                sprintf(filename, "%03i.jpg", count);
                imagep = fopen(filename, "w");
                count++;
            }
            // If JPEG has been found, write to file
            if (!(count == 0))
            {
                fwrite(&buffer, 512, 1, imagep);
            }
        }
        fclose(inputp);
        fclose(imagep);
        return 0;
    }
}
