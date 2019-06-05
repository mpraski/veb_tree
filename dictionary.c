//
// Created by marcin on 6/4/19.
//

#include "dictionary.h"

static void default_destructor(__attribute__((unused)) void *p) {}

dictionary *dictionary_new() {
  dictionary *d = (dictionary *) malloc(sizeof(dictionary));
  d->size = 0;
  memset(&d->contents, 0, DICT_SIZE * sizeof(dictionary_entry *));
  return d;
}

void dictionary_free(dictionary *d) {
  dictionary_destruct(d, default_destructor);
}

void dictionary_destruct(dictionary *d, void (*destructor)(void *)) {
  dictionary_entry *prev, *next;
  for (size_t i = 0; i < DICT_SIZE; ++i) {
    if ((prev = d->contents[i])) {
      do {
        next = prev->next;
        destructor(prev->value);
        free(prev);
        prev = next;
      } while (prev);
    }
  }
  free(d);
}

void dictionary_put(dictionary *d, uint64_t key, void *value) {
  dictionary_entry **entry = &d->contents[key % DICT_SIZE];
  if (!(*entry)) {
    *entry = (dictionary_entry *) malloc(sizeof(dictionary_entry));
    (*entry)->next = NULL;
    (*entry)->key = key;
    (*entry)->value = value;
    d->size++;
  } else {
    dictionary_entry *next = *entry, *prev;
    while (next && next->key != key) {
      prev = next;
      next = next->next;
    }

    if (!next) {
      next = (dictionary_entry *) malloc(sizeof(dictionary_entry));
      next->next = NULL;
      next->key = key;
      prev->next = next;
      d->size++;
    }

    next->value = value;
  }
}

bool dictionary_get(dictionary *d, uint64_t key, void **value) {
  dictionary_entry *entry = d->contents[key % DICT_SIZE];
  if (entry) {
    while (entry && entry->key != key) {
      entry = entry->next;
    }
    if (!entry) {
      return false;
    }
    *value = entry->value;
    return true;
  }
  return false;
}

void dictionary_print(dictionary *d) {
  dictionary_entry *next;
  for (size_t i = 0; i < DICT_SIZE; ++i) {
    if ((next = d->contents[i])) {
      printf("Cell %zu:", i);
      do {
        printf(" (%lu, %p)", next->key, next->value);
        next = next->next;
      } while (next);
      printf("\n");
    }
  }
}