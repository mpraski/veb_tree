#include <stdio.h>
#include "dictionary.h"
#include "veb_tree.h"

int main() {
  veb_tree *veb = veb_tree_new();
  for (uint64_t i = 0; i < (1u << 12u); ++i) {
    printf("i: %lu\n", i);
    veb_tree_insert(veb, i);
  }
  int64_t val;
  veb_tree_predecessor(veb, 100, &val);
  printf("Predecessor to 100: %lu\n", val);
  veb_tree_free(veb);
  return 0;
}