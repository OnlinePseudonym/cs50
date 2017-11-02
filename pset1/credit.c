#include <stdio.h>
#include <cs50.h>
#include <math.h>

int nDigits(long long i);

int main(void)
{
    // prompt user for and log cc #
    printf("card number: ");
    long long cardNumber = get_long_long();
    // get first digit of cc # for visa validation
    int firstDigit = (int) (cardNumber / (long long) pow(10, (nDigits(cardNumber) - 1)));
    // get first two digits for AMEX/MC validation
    int firstTwoDigits = (int) (cardNumber / (long long) pow(10, (nDigits(cardNumber) - 2)));
    // initialize counter for validation
    int sum = 0;
    // reject and reprompt if input is too long
    while (nDigits(cardNumber) > 16)
    {
        printf("error: card number too long. try again\n");
        printf("card number: ");
        cardNumber = get_long_long();
    };
    // add digits to sum based on Luhn's algorithm
    for (int i = 0; i < nDigits(cardNumber); i++)
    {
        // get current digit using modulo and pow() and type casting to int
        int curDigit = (cardNumber % (long long) pow(10, (i + 1))) / (long long) pow(10, i);
        //check whether index is even or odd to decide how to proceed with Luhn's algorithm
        if (i % 2 == 0)
        {
            sum += curDigit;
        }
        else
        {
            // if current digit * times is two digits use modulo to get the last digit
            // and add one because we no the number is between 12 and 18
            if (curDigit * 2 > 9)
            {
                sum += ((curDigit * 2) % 10);
                sum += 1;
            }
            else
            {
                sum += (curDigit * 2);
            };
        };
    };
    // check for known visa characteristics
    if ((firstDigit == 4) && (sum % 10 == 0) && ((nDigits(cardNumber) == 13)
            || (nDigits(cardNumber) == 16)))
    {
        printf("VISA\n");
    }
    // check for known amex characteristics
    else if (((firstTwoDigits == 34) || (firstTwoDigits == 37)) && (sum % 10 == 0)
             && (nDigits(cardNumber) == 15))
    {
        printf("AMEX\n");
    }
    // check for known mc characteristics
    else if (((firstTwoDigits > 50) && (firstTwoDigits < 56)) && (sum % 10 == 0)
             && (nDigits(cardNumber) == 16))
    {
        printf("MASTERCARD\n");
    }
    // all other cases are invalid
    else
    {
        printf("INVALID\n");
    }
};

// function to get length of the cc number
int nDigits(long long i)
{
    if (i < 10)
    {
        return 1;
    };
    if (i < 100)
    {
        return 2;
    };
    if (i < 1000)
    {
        return 3;
    };
    if (i < 10000)
    {
        return 4;
    };
    if (i < 100000)
    {
        return 5;
    };
    if (i < 1000000)
    {
        return 6;
    };
    if (i < 10000000)
    {
        return 7;
    };
    if (i < 100000000)
    {
        return 8;
    };
    if (i < 1000000000)
    {
        return 9;
    };
    if (i < 10000000000)
    {
        return 10;
    };
    if (i < 100000000000)
    {
        return 11;
    };
    if (i < 1000000000000)
    {
        return 12;
    };
    if (i < 10000000000000)
    {
        return 13;
    };
    if (i < 100000000000000)
    {
        return 14;
    };
    if (i < 1000000000000000)
    {
        return 15;
    };
    if (i < 10000000000000000)
    {
        return 16;
    };
    return 17;
};
