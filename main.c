#include <stdio.h>
#include "dictionary.h"
#include "veb_tree.h"

int main() {
  printf("Hello, World!\n");
  dictionary *d = dictionary_new();
  dictionary_put(d, 1, NULL);
  dictionary_put(d, 2, NULL);
  dictionary_put(d, 4097, NULL);
  dictionary_put(d, 4098, NULL);
  dictionary_print(d);
  dictionary_free(d);

  veb_tree *veb = veb_tree_new();
  veb_tree_insert(veb, 10);
  veb_tree_insert(veb, 11);
  veb_tree_insert(veb, 12);
  printf("Predecessor to 11: %ld\n", veb_tree_predecessor(veb, 11));
  veb_tree_free(veb);
  return 0;
}