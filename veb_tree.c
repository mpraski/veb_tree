//
// Created by marcin on 6/4/19.
//

#include "veb_tree.h"

veb_tree *veb_tree_new() {
  veb_tree *t = (veb_tree *) malloc(sizeof(veb_tree));
  t->min = 0;
  t->max = 0;
  t->empty = true;
  t->summary = (veb_tree *) malloc(sizeof(veb_tree));
  t->summary->min = 0;
  t->summary->max = 0;
  t->summary->empty = true;
  t->summary->clusters = dictionary_new();
  t->clusters = dictionary_new();
  return t;
}

void veb_tree_free(veb_tree *t) {
  dictionary_destruct(t->clusters, (void (*)(void *)) veb_tree_free);
  dictionary_destruct(t->summary->clusters, (void (*)(void *)) veb_tree_free);
  free(t->summary);
  free(t);
}

void veb_tree_insert(veb_tree *t, uint64_t value) {
  _veb_tree_insert(t, value, sizeof(value) * CHAR_BIT);
}

void _veb_tree_insert(veb_tree *t, uint64_t value, unsigned bits) {
  if (!t) {
    t = veb_tree_new();
  }

  if (t->empty) {
    t->min = t->max = value;
    t->empty = false;
    return;
  }

  if (value < t->min) {
    uint64_t temp = t->min;
    t->min = value;
    value = temp;
  } else if (value > t->max) {
    t->max = value;
  }

  if (bits > 2) {
    veb_tree *child;
    uint64_t lo, hi;
    unsigned h = bits >> 2u;
    lo_hi(value, h, &lo, &hi);

    if (!dictionary_get(t->clusters, lo, (void **) &child)) {
      child = veb_tree_new();
      dictionary_put(t->clusters, lo, child);
    }

    if (child->empty) {
      _veb_tree_insert(t->summary, lo, h);
    }

    _veb_tree_insert(child, hi, h);
  }
}

bool veb_tree_predecessor(veb_tree *t, uint64_t value, uint64_t *result) {
  return _veb_tree_predecessor(t, value, sizeof(value) * CHAR_BIT, result);
}

bool _veb_tree_predecessor(veb_tree *t, uint64_t value, unsigned bits, uint64_t *result) {
  if (value <= t->min) {
    return false;
  } else if (value > t->max) {
    *result = t->max;
    return true;
  }

  veb_tree *child;
  uint64_t lo, hi;
  unsigned h = bits >> 2u;
  lo_hi(value, h, &lo, &hi);

  dictionary_get(t->clusters, lo, (void **) &child);

  if (value > child->max) {
    *result = child->max;
    return true;
  } else if (hi <= child->min) {
    if (_veb_tree_predecessor(t->summary, lo, h, result)) {
      dictionary_get(t->clusters, *result, (void **) &child);
      *result = child->max;
    } else {
      *result = t->min;
    }
    return true;
  }

  return _veb_tree_predecessor(child, hi, h, result);
}

void veb_tree_print(veb_tree *t, unsigned indent) {
  printf("%*sMin: %lu, Max: %lu, Empty: %d\n", indent, "", t->min, t->max, t->empty);
  printf("%*sSummary:\n", indent, "");
  printf("%*sMin: %lu, Max: %lu, Empty: %d\n", indent + 4, "", t->summary->min, t->summary->max, t->summary->empty);

  _veb_tree_print(t->summary->clusters, indent + 4);
  _veb_tree_print(t->clusters, indent);
}

void _veb_tree_print(dictionary *d, unsigned indent) {
  if (dictionary_empty(d)) {
    printf("%*sClusters are empty.\n", indent, "");
  } else {
    printf("%*sClusters:\n", indent, "");
  }

  dictionary_entry *next;
  for (size_t i = 0; i < DICT_SIZE; ++i) {
    if ((next = d->contents[i])) {
      do {
        veb_tree_print((veb_tree *) next->value, indent + 4);
        next = next->next;
      } while (next);
    }
  }
}