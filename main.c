#include <stdio.h>
#include "dictionary.h"

int main() {
  printf("Hello, World!\n");
  dictionary *d = new_dictionary();
  dictionary_put(d, 1, NULL);
  dictionary_put(d, 2, NULL);
  dictionary_put(d, 4097, NULL);
  dictionary_put(d, 4098, NULL);
  dictionary_print(d);
  free_dictionary(d);
  return 0;
}