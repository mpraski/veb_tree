#ifndef VEB_TREE_H
#define VEB_TREE_H

#include <stdint.h>
#include <stdio.h>
#include "dictionary.h"

struct veb_tree {
    uint64_t min;
    uint64_t max;

    struct veb_tree *summary;

    dictionary *clusters;
};

typedef struct veb_tree veb_tree;

veb_tree *veb_tree_new();

void veb_tree_free(veb_tree *t);

void veb_tree_insert(veb_tree *t, uint64_t value);

void _veb_tree_insert(veb_tree *t, uint64_t value, int bits);

uint64_t veb_tree_predecessor(veb_tree *t, uint64_t value);

uint64_t veb_tree_successor(veb_tree *t, uint64_t value);

uint64_t _veb_tree_predecessor(veb_tree *t, uint64_t value, int bits);

static inline void lo_hi(uint64_t val, uint8_t bits, uint64_t *lo, uint64_t *hi) {
  *lo = ((((1u << bits) - 1u) << bits) & val) >> bits;
  *hi = ((1u << bits) - 1u) & val;
}

#endif //VEB_TREE_H