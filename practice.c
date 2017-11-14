
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
    char *memCard = argv[2];

    // open input file
    FILE *inptr = fopen(memCard, "r");

    // test input
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", memCard);
        return 2;
    }

    //
    bool jpegOpen = false;

    //
    for (;;)
    {
        // read input in blocks of 512 bytes
        BYTE buffer[512];
        fread(&buffer, 1, 512, inptr);
        size_t n = fread(&buffer, 1, 512, inptr);

        //
        int fileCount = 0;
        FILE *img;

        //
        if (isNewJpeg(buffer))
        {
            //
            char filename[sizeof "000.jpg"];
            sprintf(filename, "%03i.jpg", fileCount);
            fileCount++;
            img = fopen(filename, "w");
            jpegOpen = true;

            //
            if (img == NULL)
            {
                fprintf(stderr, "Could not open %s.\n", img);
                return 3;
            }

            //
            fwrite(&buffer, 1, 512, img);

        }

        //
        if (jpegOpen)
        {
            fwrite(&buffer, 1, 512, img);
        }

        //
        if (n < 512)
        {
            break;
        }
    }
}

bool isNewJpeg(BYTE byte[512])
{
    if (byte[0] == 0xff && byte[1] == 0xd8 && byte[2] == 0xff && (byte[3] & 0xf0) == 0xe0)
    {
        return true;
    }

    return false;
}

/*bool isQualFourth(char *byte)
{

    if ((byte == 0xe0) || (byte == 0xe1) || (byte == 0xe2) || (byte == 0xe3) || (byte == 0xe4) || (byte == 0xe5) ||
        (byte == 0xe6) || (byte == 0xe7) || (byte == 0xe8) || (byte == 0xe9) || (byte == 0xea) || (byte == 0xeb) ||
        (byte == 0xec) || (byte == 0xed) || (byte == 0xee) || (byte == 0xef))
    {
        return true;
    }

    return false;
}*/
