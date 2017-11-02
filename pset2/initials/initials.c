#include <stdio.h>
#include <ctype.h>
#include <cs50.h>
#include <string.h>

int main(void)
{
    string s = get_string();
    for (int i = 0, n = strlen(s); i < n; i++)
    {
        while (s[i] == ' ')
        {
            i++;
        }
        if (s[i] != '\0')
        {
            printf("%c", toupper(s[i]));

        }
        while (s[i] != ' ')
        {
            i++;
            if (i > n)
            {
                break;
            }
        }
    }
    printf("\n");
}
