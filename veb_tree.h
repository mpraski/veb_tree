#ifndef VEB_TREE_H
#define VEB_TREE_H

#include <stdint.h>
#include <stdio.h>
#include <limits.h>
#include "dictionary.h"

struct veb_tree {
    uint64_t min;
    uint64_t max;
    struct veb_tree *summary;
    dictionary *clusters;
    bool empty;
};

typedef struct veb_tree veb_tree;

veb_tree *veb_tree_new();

void veb_tree_free(veb_tree *t);

void veb_tree_insert(veb_tree *t, uint64_t value);

void _veb_tree_insert(veb_tree *t, uint64_t value, unsigned bits);

uint64_t veb_tree_predecessor(veb_tree *t, uint64_t value);

uint64_t _veb_tree_predecessor(veb_tree *t, uint64_t value, unsigned bits);

uint64_t veb_tree_successor(veb_tree *t, uint64_t value);

void veb_tree_print(veb_tree *t, unsigned indent);

void _veb_tree_print(dictionary *d, unsigned indent);

static inline void lo_hi(
    uint64_t val,
    uint64_t bits,
    uint64_t *lo,
    uint64_t *hi
) {
  *lo = val >> bits;
  *hi = (((uint64_t) 1u << bits) - (uint64_t) 1u) & val;
}

#endif //VEB_TREE_H