#define _XOPEN_SOURCE

#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <unistd.h>

int main(int argc, string argv[])
{
    // kill program if arguments fall short of or exceed 2
    if (argc != 2)
    {
        printf("ERROR: program limited to one alpha command-line argument\n");
        return 1;
    };
    // given possible chars for passwords.
    char *seedchars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    string pw = argv[argc - 1];
    char pass[5];
    char salt[3] = {pw[0], pw[1]};
    bool isMatch = false;
    for (int i = 0, n = strlen(seedchars); i < n; i++)
    {
        // create a new string and copy it to pass so it stored globaly and can
        // be returned to the user outside the for loop
        char temp[2] = {seedchars[i]};
        strcpy(pass, temp);
        // crypt the generated string
        string result = crypt(pass, salt);
        // test generated crypt against given hashed pw and if match break loop
        if (strcmp (result, pw) == 0)
        {
            i = 1000;
            isMatch = true;
            break;
        }
    }
    // test if pw is already found
    // comments from first iteration pertain here
    if (!isMatch)
    {
        for (int i = 0, n = strlen(seedchars); i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                char temp[3] = {seedchars[i], seedchars[j]};
                strcpy(pass, temp);
                string result = crypt(pass, salt);
                if (strcmp (result, pw) == 0)
                {
                    i = j = 1000;
                    isMatch = true;
                    break;
                }
            }
        }
    }
    // test if pw is already found
    // comments from first iteration pertain here
    if (!isMatch)
    {
        for (int i = 0, n = strlen(seedchars); i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                for (int k = 0; k < n; k++)
                {
                    char temp[4] = {seedchars[i], seedchars[j], seedchars[k]};
                    strcpy(pass, temp);
                    string result = crypt(pass, salt);
                    if (strcmp (result, pw) == 0)
                    {
                        i = j = k = 1000;
                        isMatch = true;
                        break;
                    }
                }
            }
        }
    }
    // test if pw is already found
    // comments from first iteration pertain here
    if (!isMatch)
    {
        for (int i = 0, n = strlen(seedchars); i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                for (int k = 0; k < n; k++)
                {
                    for (int l = 0; l < n; l++)
                    {
                        char temp[5] = {seedchars[i], seedchars[j], seedchars[k], seedchars[l]};
                        strcpy(pass, temp);
                        string result = crypt(pass, salt);
                        if (strcmp (result, pw) == 0)
                        {
                            i = j = k = l = 1000;
                            isMatch = true;
                            break;
                        }
                    }
                }
            }
        }
    }
    printf("%s\n", pass);
    return 0;
}
