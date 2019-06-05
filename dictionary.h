//
// Created by marcin on 6/4/19.
//

#ifndef VEB_TREE_DICTIONARY_H
#define VEB_TREE_DICTIONARY_H

#define DICT_SIZE 4096U

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

struct dictionary_entry {
    struct dictionary_entry *next;
    uint64_t key;
    void *value;
};

typedef struct dictionary_entry dictionary_entry;

struct dictionary {
    size_t size;
    struct dictionary_entry *contents[DICT_SIZE];
};

typedef struct dictionary dictionary;

dictionary *new_dictionary();

void free_dictionary(dictionary *d);

void dictionary_put(dictionary *d, uint64_t key, void *value);

bool dictionary_get(dictionary *d, uint64_t key, void **value);

void dictionary_print(dictionary *d);

#endif //VEB_TREE_DICTIONARY_H
