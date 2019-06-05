//
// Created by marcin on 6/4/19.
//

#include "veb_tree.h"

veb_tree *new_veb_tree() {
  veb_tree *t = (veb_tree *) malloc(sizeof(veb_tree));
  t->min = 0;
  t->max = 0;
  t->summary = (veb_tree *) malloc(sizeof(veb_tree));
  t->clusters = new_dictionary();
  return t;
}

void veb_insert(veb_tree *t, uint64_t value) {
  _veb_insert(t, value, 64);
}

void _veb_insert(veb_tree *t, uint64_t value, uint8_t bits) {
  if (!t->clusters->size) {
    t->min = value;
  } else if (value < t->min) {
    uint64_t temp = t->min;
    t->min = value;
    value = temp;

    uint64_t lo, hi;
    uint8_t h = bits / 2;
    lo_hi(value, h, &lo, &hi);
  }
}

uint64_t veb_predecessor(veb_tree *t, uint64_t value) {
  return _veb_predecessor(t, value, 64);
}

uint64_t _veb_predecessor(veb_tree *t, uint64_t value, uint8_t bits) {
  if (value > t->max) {
    return t->max;
  }

  veb_tree *child;
  uint64_t lo, hi;
  uint8_t h = bits / 2;
  lo_hi(value, h, &lo, &hi);

  if (!dictionary_get(t->clusters, lo, (void **) &child)) {
    fprintf(stderr, "Key %lu not found in dictionary\n", lo);
  } else if (child->min < value) {
    return _veb_predecessor(child, hi, h);
  }

  uint64_t cp = _veb_predecessor(t->summary, lo, h);
  if (!dictionary_get(t->clusters, cp, (void **) &child)) {
    fprintf(stderr, "Key %lu not found in dictionary\n", cp);
  }

  return child->max;
}