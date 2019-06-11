#include <stdio.h>
#include "dictionary.h"
#include "veb_tree.h"

void shuffle(uint64_t *array, unsigned n);

int main() {
  unsigned sz = 200;
  uint64_t *vals = (uint64_t *) malloc(sz * sizeof(uint64_t));
  for (unsigned i = 0; i < sz; ++i) {
    vals[i] = i;
  }

  shuffle(vals, sz);
  veb_tree_sort_descending(vals, sz);

  for (unsigned i = 0; i < sz; ++i) {
    printf("%u: %lu\n", i, vals[i]);
  }
  return 0;
}

void shuffle(uint64_t *array, unsigned n) {
  if (n > 1) {
    unsigned i;
    for (i = 0; i < n - 1; i++) {
      unsigned j = i + rand() / (RAND_MAX / (n - i) + 1);
      uint64_t t = array[j];
      array[j] = array[i];
      array[i] = t;
    }
  }
}