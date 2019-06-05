//
// Created by marcin on 6/4/19.
//

#include "veb_tree.h"

veb_tree *veb_tree_new() {
  veb_tree *t = (veb_tree *) malloc(sizeof(veb_tree));
  t->min = 0;
  t->max = 0;
  t->summary = (veb_tree *) malloc(sizeof(veb_tree));
  t->clusters = dictionary_new();
  return t;
}

void veb_tree_free(veb_tree *t) {
  dictionary_destruct(t->clusters, (void (*)(void *)) veb_tree_free);
  free(t->summary);
  free(t);
}

void veb_tree_insert(veb_tree *t, uint64_t value) {
  _veb_tree_insert(t, value, 64);
}

void _veb_tree_insert(veb_tree *t, uint64_t value, int bits) {
  if (!t->clusters->size) {
    t->min = t->max = value;
    return;
  }

  if (value < t->min) {
    uint64_t temp = t->min;
    t->min = value;
    value = temp;
  } else if (value > t->max) {
    t->max = value;
  }

  veb_tree *child;
  uint64_t lo, hi;
  int h = bits / 2;
  lo_hi(value, h, &lo, &hi);

  if (!dictionary_get(t->clusters, hi, (void **) &child)) {
    child = veb_tree_new();
    dictionary_put(t->clusters, hi, child);
  }

  _veb_tree_insert(child, lo, h);

  if (child->min == child->max) {
    _veb_tree_insert(t->summary, hi, h);
  }
}

uint64_t veb_tree_predecessor(veb_tree *t, uint64_t value) {
  return _veb_tree_predecessor(t, value, 64);
}

uint64_t _veb_tree_predecessor(veb_tree *t, uint64_t value, int bits) {
  if (value > t->max) {
    return t->max;
  }

  veb_tree *child;
  uint64_t lo, hi;
  int h = bits / 2;
  lo_hi(value, h, &lo, &hi);

  if (!dictionary_get(t->clusters, lo, (void **) &child)) {
    fprintf(stderr, "Key %lu not found in dictionary\n", lo);
  } else if (child->min < value) {
    return _veb_tree_predecessor(child, hi, h);
  }

  uint64_t cp = _veb_tree_predecessor(t->summary, lo, h);
  if (!dictionary_get(t->clusters, cp, (void **) &child)) {
    fprintf(stderr, "Key %lu not found in dictionary\n", cp);
  }

  return child->max;
}