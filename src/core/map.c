#include "labster/core/map.h"
#include "labster/core/list.h"

void *_mapGet(void *map, const void *key, MapEqFunc eqFunc) {
  for (size_t i = 0; i < listLength(map); i++) {
    Map(void*, void*) pair = listAt(map, i);
    if (eqFunc(pair->key, key))
      return &pair->value;
  }
  return NULL;
}

bool ptrEq(const void **a, const void **b) {
  return *a == *b;
}

