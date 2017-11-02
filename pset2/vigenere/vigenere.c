#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    // kill program if arguments fall short of or exceed 2
    if (argc != 2)
    {
        printf("ERROR: program limited to one alpha command-line argument\n");
        return 1;
    };
    // iterate over argv[i] and ensure alpha otherwise kill program
    for (int i = 0, j = strlen(argv[1]); i < j; i++)
    {
        if (!isalpha(argv[1][i]))
        {
            printf("ERROR: program limited to one alpha command-line argument\n");
            return 1;
        };
    };
    // prompt user for message to be ciphered
    printf("plaintext: ");
    string p = get_string();
    // print ciphertext
    printf("ciphertext: ");
    // iterate over each char in the user input
    for (int i = 0, j = 0, n = strlen(p); i < n; i++)
    {
        // set shift based on key, and make sure it is reused from begining
        int shift = (toupper(argv[1][j % strlen(argv[1])]) - 65);
        // test for alpha
        if (isalpha(p[i]))
        {
            // test for upper and apply shift and increase by value of 'A'
            if (isupper(p[i]))
            {
                printf("%c", (char)((((p[i] - 65) + shift) % 26) + 65));
                j++;
            }
            // if alpha and not upper must be lower so apply shift to increase by the value of 'a'
            else
            {
                printf("%c", (char)((((p[i] - 97) + shift) % 26) + 97));
                j++;
            };
        }
        // pass on any non alpha character
        else
        {
            printf("%c", p[i]);
        }
    };
    // newline
    printf("\n");
}
