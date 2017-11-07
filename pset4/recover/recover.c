#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "bmp.h"

bool isNewJpeg(BYTE byte[512]);
//bool isQualFourth(char *byte);

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "usage: ./recover <filename>");
        return 1;
    }

    // define raw input
    char *memCard = argv[1];

    // open input file
    FILE *inptr = fopen(memCard, "r");

    // test input
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", memCard);
        return 2;
    }

    // initialize
    BYTE buffer[512];
    char filename[sizeof "000.jpg"];
    bool jpegOpen = false;
    int fileCount = 0;
    FILE *img = NULL;

    //
    for (;;)
    {
        // read input
        size_t n = fread(&buffer, 512, 1, inptr);

        // test for jpeg signature
        if (isNewJpeg(buffer))
        {
            // see if jpeg open and close if it is
            if (jpegOpen)
            {
                fclose(img);
            }
            // if 1st jpeg set jpegOpen to true
            else
            {
                jpegOpen = true;
            }
            // open file and name it based on counter
            sprintf(filename, "%03i.jpg", fileCount);
            fileCount++;
            img = fopen(filename, "w");

            // ensure file opened correctly
            if (img == NULL)
            {
                fprintf(stderr, "Could not open %s.\n", filename);
                return 3;
            }

            // write the buffer to img
            fwrite(&buffer, 512, 1, img);
        }
        else if (jpegOpen)
        {
            // write the buffer to img
            fwrite(&buffer, 512, 1, img);
        }

        // if fread doesn't find a block terminate loop
        if (n < 1)
        {
            break;
        }
    }

    // close open files and return 0
    fclose(inptr);
    fclose(img);
    return 0;
}

// test for jpeg signature in first 4 bytes of block
bool isNewJpeg(BYTE byte[512])
{
    if (byte[0] == 0xff && byte[1] == 0xd8 && byte[2] == 0xff && (byte[3] & 0xf0) == 0xe0)
    {
        return true;
    }
    return false;
}
