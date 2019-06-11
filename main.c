#include <stdio.h>
#include <time.h>
#include "dictionary.h"
#include "veb_tree.h"

void shuffle(uint64_t *array, size_t n);

int cmpfunc(const void *a, const void *b) {
  return (*(uint64_t *) b - *(uint64_t *) a);
}

int main() {
  size_t sz = 10000;
  uint64_t *vals = (uint64_t *) malloc(sz * sizeof(uint64_t));
  uint64_t *vals_dup = (uint64_t *) malloc(sz * sizeof(uint64_t));
  for (unsigned i = 0; i < sz; ++i) {
    vals[i] = i;
  }

  shuffle(vals, sz);
  memcpy(vals_dup, vals, sz * sizeof(uint64_t));

  clock_t start, end;
  double cpu_time_used;

  start = clock();
  veb_tree_sort_descending(vals, sz);
  end = clock();
  cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
  printf("veb_tree_sort_descending() took %f seconds to execute \n", cpu_time_used);

  start = clock();
  qsort(vals_dup, sz, sizeof(uint64_t), cmpfunc);
  end = clock();
  cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
  printf("qsort() took %f seconds to execute \n", cpu_time_used);

  return 0;
}

void shuffle(uint64_t *array, size_t n) {
  if (n > 1) {
    size_t i;
    for (i = 0; i < n - 1; i++) {
      size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
      uint64_t t = array[j];
      array[j] = array[i];
      array[i] = t;
    }
  }
}