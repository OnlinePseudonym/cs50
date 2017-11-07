/**
 * Copies a BMP piece by piece, just because.
 */

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize <multiplier> <infile> <outfile>\n");
        return 1;
    }

    // ensure between 1 & 100
    if ((argv[1] < 1) || (argv[1] > 101))
    {
        fprintf(stderr, "Multiplier must be between 1 and 100");
        return 1;
    }

    // define argurments
    int multiplier = atoi(argv[1]);
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // initialize header copies to edit
    BITMAPFILEHEADER outbf;
    outbf = bf;
    BITMAPINFOHEADER outbi;
    outbi = bi;

        // multiply height and width of bitmap update size attributes
    outbi.biHeight *= multiplier;
    outbi.biWidth *= multiplier;
    printf("%" PRId32 "\n", bi.biWidth);
    printf("%" PRId32 "\n", outbi.biWidth);

    // determine padding for scanlines
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // determine output padding
    int outPadding = (4 - (outbi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    printf("%i\n", outPadding);

    outbi.biSizeImage = ((sizeof(RGBTRIPLE) * outbi.biWidth) + outPadding) * abs(outbi.biHeight);
    outbf.bfSize = outbi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // write outfile's BITMAPFILEHEADER
    fwrite(&outbf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&outbi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {

        for (int j = 0; j < multiplier; j++)
        {

            long loopStart = ftell(inptr);

            // iterate over pixels in scanline
            for (int k = 0; k < bi.biWidth; k++)
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                // write RGB triple to outfile
                for (int l = 0; l < multiplier; l++)
                {
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            }

            // add padding
            for (int m = 0; m < outPadding; m++)
            {
                fputc(0x00, outptr);
            }

            if (j < multiplier - 1)
            {
                // send pointer back to begining of scanline
                fseek(inptr, loopStart, SEEK_SET);
            }
            else
            {
                // skip over input padding
                fseek(inptr, padding, SEEK_CUR);
            }

        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
