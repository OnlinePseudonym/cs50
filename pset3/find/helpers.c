/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */

#include <cs50.h>
#include <stdio.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    if (value < 0)
    {
        return false;
    }
    /*for (int i = 0; i < n; i++)
    {
        if (values[i] == value)
        {
            return true;
        }
    }*/



    if (n < 2)
    {
        if (values[0] == value)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    int lastIndex = n - 1;
    int middle = (lastIndex / 2) + 1;
    printf("%i\n", middle);
    printf("%i\n", values[middle]);
    int upperBound = n - middle;
    if (values[middle] == value)
    {
        return true;
    }
    else if (value < values[middle])
    {
        return search(value, values, middle);
    }
    else
    {
        return search(value, values + middle, upperBound);
    }
    return false;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    int max = 65536;
    int * count = calloc(max, sizeof(int));

    for (int i = 0; i < n; i++)
    {
        count[values[i]] += 1;
    }
    for (int i = 0, j = 0; i < max; i++)
    {
        while (count[i] > 0)
        {
            values[j] = i;
            count[i] = count[i] - 1;
            j++;
        }
        if (j == n){
            break;
        }
    }
}
