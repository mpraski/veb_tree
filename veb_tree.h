#ifndef VEB_TREE_H
#define VEB_TREE_H

#include <stdint.h>
#include <stdio.h>
#include <limits.h>
#include "dictionary.h"

#define NONE (uint64_t)(-1)
#define UNIVERSE_BITS (sizeof(uint64_t) * CHAR_BIT)

struct veb_tree {
    uint64_t min;
    uint64_t max;

    struct veb_tree *summary;

    dictionary *clusters;
};

typedef struct veb_tree veb_tree;

veb_tree *veb_tree_new(unsigned bits);

void veb_tree_free(veb_tree *t);

void veb_tree_insert(veb_tree *t, uint64_t value);

void _veb_tree_insert(veb_tree **t, uint64_t value, unsigned bits);

uint64_t veb_tree_predecessor(veb_tree *t, uint64_t value);

uint64_t _veb_tree_predecessor(veb_tree *t, uint64_t value, unsigned bits);

uint64_t veb_tree_successor(veb_tree *t, uint64_t value);

uint64_t _veb_tree_successor(veb_tree *t, uint64_t value, unsigned bits);

void veb_tree_sort_descending(uint64_t *values, unsigned size);

void veb_tree_print(veb_tree *t, unsigned indent);

void _veb_tree_print(dictionary *d, unsigned indent);

static inline bool veb_tree_empty(veb_tree *t) {
  return t->min == NONE;
}

static inline uint64_t veb_tree_min(veb_tree *t) {
  return t ? t->min : NONE;
}

static inline uint64_t veb_tree_max(veb_tree *t) {
  return t ? t->max : NONE;
}

static inline void lo_hi(
    uint64_t val,
    uint64_t bits,
    uint64_t *lo,
    uint64_t *hi
) {
  *lo = (((uint64_t) 1u << bits) - (uint64_t) 1u) & val;
  *hi = val >> bits;
}

static inline uint64_t whole(
    uint64_t lo,
    uint64_t hi,
    uint64_t bits
) {
  return (hi << bits) + lo;
}

static inline size_t veb_dict_capacity(unsigned bits) {
  switch (bits) {
    case 64:
      return 1u << 24u;
    case 32:
      return 1u << 20u;
    default:
      return 1u << 16u;
  }
}

#endif //VEB_TREE_H