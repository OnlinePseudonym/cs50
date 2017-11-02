#include <stdio.h>
#include <cs50.h>

int main(void)
{
    printf("Enter desired height of the pyramid\n");
    int height = get_int();
    // set bounds for input and reprompt if not met
    while (height > 23 || height < 0)
    {
        printf("Invalid entry: please re-enter height\n");
        height = get_int();
    };
    // iterate throught each row and define output
    for (int row = 0; row < height; row++)
    {
        // print each char on left side of pyramid
        for (int left = 0; left < height; left++)
        {
            if (left < height - (row + 1))
            {
                printf(" ");
            }
            else
            {
                printf("#");
            };
        };
        // print gap
        printf("  ");
        // print each char on right side of pyramid
        for (int right = 0; right < height; right++)
        {
            if (right < row + 1)
            {
                printf("#");
            };
        };
        // move to a new line after all characters are printed
        // and repeat until all rows are finished
        printf("\n");
    };
};
