//
// Created by marcin on 6/4/19.
//

#include "veb_tree.h"

veb_tree *veb_tree_new(unsigned bits) {
  veb_tree *t = (veb_tree *) malloc(sizeof(veb_tree));
  t->min = NONE;
  t->max = 0;
  t->summary = NULL;
  t->clusters = dictionary_new(veb_dict_capacity(bits));
  return t;
}

void veb_tree_free(veb_tree *t) {
  dictionary_destruct(t->clusters, (void (*)(void *)) veb_tree_free);
  if (t->summary) {
    dictionary_destruct(t->summary->clusters, (void (*)(void *)) veb_tree_free);
    free(t->summary);
  }
  free(t);
}

void veb_tree_insert(veb_tree *t, uint64_t value) {
  _veb_tree_insert(&t, value, UNIVERSE_BITS / 2);
}

void _veb_tree_insert(veb_tree **tree, uint64_t value, unsigned bits) {
  if (!(*tree)) {
    *tree = veb_tree_new(bits);
  }

  veb_tree *t = *tree;

  if (veb_tree_empty(t)) {
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

  if (bits) {
    veb_tree *child;
    uint64_t lo, hi;
    lo_hi(value, bits, &lo, &hi);

    if (!(child = dictionary_get(t->clusters, hi))) {
      child = veb_tree_new(bits);
      dictionary_put(t->clusters, hi, child);
      _veb_tree_insert(&t->summary, hi, bits / 2);
    }

    _veb_tree_insert(&child, lo, bits / 2);
  }
}

uint64_t veb_tree_predecessor(veb_tree *t, uint64_t value) {
  return _veb_tree_predecessor(t, value, UNIVERSE_BITS / 2);
}

uint64_t _veb_tree_predecessor(veb_tree *t, uint64_t value, unsigned bits) {
  if (value > t->max) {
    return t->max;
  }

  if (!t->summary) {
    if (value > t->min) {
      return t->min;
    }

    return NONE;
  }

  veb_tree *child;
  uint64_t lo, hi, lo2, hi2;
  lo_hi(value, bits, &lo, &hi);
  child = dictionary_get(t->clusters, hi);
  hi2 = hi;

  if (lo > veb_tree_min(child)) {
    if ((lo2 = _veb_tree_predecessor(child, lo, bits / 2)) == NONE) {
      return NONE;
    }
  } else {
    if ((hi2 = _veb_tree_predecessor(t->summary, hi, bits / 2)) == NONE) {
      if (t->min < value) {
        return t->min;
      }
      return NONE;
    }
    child = dictionary_get(t->clusters, hi2);
    if ((lo2 = veb_tree_max(child)) == NONE) {
      return NONE;
    }
  }

  return whole(lo2, hi2, bits);
}

uint64_t veb_tree_successor(veb_tree *t, uint64_t value) {
  return _veb_tree_successor(t, value, 32);
}

uint64_t _veb_tree_successor(veb_tree *t, uint64_t value, unsigned bits) {
  if (value < t->min) {
    return t->min;
  }

  if (!t->summary) {
    if (value < t->max) {
      return t->max;
    }
  }
}

void veb_tree_sort_descending(uint64_t *values, unsigned size) {
  size_t i = 0;
  veb_tree *veb = veb_tree_new(UNIVERSE_BITS);
  while (i < size) {
    veb_tree_insert(veb, values[i++]);
  }

  i = 1;
  uint64_t pred = values[0] = veb->max;
  while ((pred = veb_tree_predecessor(veb, pred)) != NONE) {
    values[i++] = pred;
  }
  veb_tree_free(veb);
}

void veb_tree_print(veb_tree *t, unsigned indent) {
  printf("%*sMin: %lu, Max: %lu, Empty: %d\n", indent, "", t->min, t->max, veb_tree_empty(t));
  if (t->summary) printf("%*sSummary:\n", indent, "");
  if (t->summary)
    printf("%*sMin: %lu, Max: %lu, Empty: %d\n", indent + 4, "", t->summary->min, t->summary->max,
           veb_tree_empty(t->summary));

  if (t->summary) _veb_tree_print(t->summary->clusters, indent + 4);
  _veb_tree_print(t->clusters, indent);
}

void _veb_tree_print(dictionary *d, unsigned indent) {
  if (dictionary_empty(d)) {
    printf("%*sClusters are empty.\n", indent, "");
  } else {
    printf("%*sClusters:\n", indent, "");
  }

  dictionary_entry *next;
  for (size_t i = 0; i < d->capacity; ++i) {
    if ((next = d->contents[i])) {
      do {
        printf("%*sKey (%lu):\n", indent, "", next->key);
        veb_tree_print((veb_tree *) next->value, indent + 4);
        next = next->next;
      } while (next);
    }
  }
}