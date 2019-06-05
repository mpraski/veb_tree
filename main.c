#include <stdio.h>
#include "dictionary.h"
#include "veb_tree.h"

int main() {
  veb_tree *veb = veb_tree_new();
  veb_tree_insert(veb, 10);
  veb_tree_insert(veb, 11);
  veb_tree_insert(veb, 12);
  veb_tree_insert(veb, 13);
  veb_tree_print(veb, 0);
  printf("Predecessor to 11: %lu\n", veb_tree_predecessor(veb, 11));
  veb_tree_free(veb);
  return 0;
}