//
// Created by marcin on 6/4/19.
//

#ifndef VEB_TREE_DICTIONARY_H
#define VEB_TREE_DICTIONARY_H

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
    size_t capacity;
    size_t size;

    struct dictionary_entry **contents;
};

typedef struct dictionary dictionary;

dictionary *dictionary_new(size_t capacity);

void dictionary_free(dictionary *d);

void dictionary_destruct(dictionary *d, void (*destructor)(void *));

void dictionary_put(dictionary *d, uint64_t key, void *value);

void *dictionary_get(dictionary *d, uint64_t key);

bool dictionary_empty(dictionary *d);

void dictionary_print(dictionary *d);

#endif //VEB_TREE_DICTIONARY_H
