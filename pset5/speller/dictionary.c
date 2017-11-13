/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "dictionary.h"

#undef get16bits

#if (defined(__GNUC__) && defined(__i386__)) || defined(__WATCOMC__) \
  || defined(_MSC_VER) || defined (__BORLANDC__) || defined (__TURBOC__)
#define get16bits(d) (*((const uint16_t *) (d)))
#endif

#if !defined (get16bits)
#define get16bits(d) ((((uint32_t)(((const uint8_t *)(d))[1])) << 8)\
                       +(uint32_t)(((const uint8_t *)(d))[0]) )
#endif

typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

node **hashTable;
int htSize;
int words = 0;

// implimentation fo superfasthash from Paul Hsieh
// http://www.azillionmonkeys.com/qed/hash.html
uint32_t SuperFastHash (const char * data, int len) {
uint32_t hash = len, tmp;
int rem;

    if (len <= 0 || data == NULL) return 0;

    rem = len & 3;
    len >>= 2;

    /* Main loop */
    for (;len > 0; len--) {
        hash  += get16bits (data);
        tmp    = (get16bits (data+2) << 11) ^ hash;
        hash   = (hash << 16) ^ tmp;
        data  += 2*sizeof (uint16_t);
        hash  += hash >> 11;
    }

    /* Handle end cases */
    switch (rem) {
        case 3: hash += get16bits (data);
                hash ^= hash << 16;
                hash ^= ((signed char)data[sizeof (uint16_t)]) << 18;
                hash += hash >> 11;
                break;
        case 2: hash += get16bits (data);
                hash ^= hash << 11;
                hash += hash >> 17;
                break;
        case 1: hash += (signed char)*data;
                hash ^= hash << 10;
                hash += hash >> 1;
    }

    /* Force "avalanching" of final 127 bits */
    hash ^= hash << 3;
    hash += hash >> 5;
    hash ^= hash << 4;
    hash += hash >> 17;
    hash ^= hash << 25;
    hash += hash >> 6;

    return hash;
}

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    int length = strlen(word);
    char query[length + 1];

    for (int i = 0; i < length; i++)
    {
        query[i] = tolower(word[i]);
    }

    query[length] = '\0';
    int hash = (SuperFastHash(query, length) % htSize);


    if (hashTable[hash] != NULL)
    {
        node *cur = hashTable[hash];

        if (strcmp(cur->word, query) == 0)
        {
            return true;
        }
        else
        {
           while (cur->next != NULL)
            {
                cur = cur->next;
                if (strcmp(cur->word, query) == 0)
                {
                    return true;
                }
            }
        }
    }
    return false;
}


/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    // load dictionary and check that load worked
    FILE *input = fopen(dictionary, "r");
    if (input == NULL)
    {
        printf("Could not open %s.\n", dictionary);
        unload();
        return false;
    }
    char c;
    while ((c = fgetc(input)) != EOF)
    {
        if (c == ' ' || c == '\n')
        {
            words++;
        }
    }
    rewind(input);
    htSize = words / LOAD_FACTOR;

    hashTable = realloc(hashTable, htSize * sizeof(node));

    for (int i = 0; i < htSize; i++)
    {
        hashTable[i] = NULL;
    }

    char *word = malloc(LENGTH + 1);

    while (fscanf(input, "%s", word) != EOF)
    {
        int length = strlen(word);
        int hash = (SuperFastHash(word, length) % htSize);

        node *entry = malloc(sizeof(node));
        strcpy(entry->word, word);

        if (hashTable[hash] == NULL)
        {
            hashTable[hash] = entry;
            entry->next = NULL;
        }
        else
        {
            entry->next = hashTable[hash];
            hashTable[hash] = entry;
        }
    }
    free(word);
    fclose(input);
    return true;
}


/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return words;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    for (int i = 0; i < htSize; i++)
    {
        node *cur = hashTable[i];
        while (cur != NULL)
        {
            node *tmp = cur->next;
            free(cur);
            cur = tmp;
        }
    }
    free(hashTable);

    return true;
}
