#pragma once

#include "list.h"
#include <stdbool.h>

typedef bool (*MapEqFunc)(const void *a, const void *b);

bool ptrEq(const void **a, const void **b);

#define Map(keyType, valueType) List(struct { keyType key; valueType value; })
#define mapNew(type) listNew(typeof(*(type)NULL))
#define mapInsert(map, key, value) listPush(map, ((typeof(*map)){key, value}))
#define mapPair(key, value) ({ key, value })
#define mapHas(map, searchKey, eqFunc) (_mapGet(map, searchKey, (MapEqFunc)eqFunc) != NULL)
#define mapGet(map, searchKey, eqFunc) (*(typeof(map->value)*)_mapGet(map, searchKey, (MapEqFunc)eqFunc))
#define mapFor listForPtr
#define mapFree listFree

void *_mapGet(void *map, const void *key, MapEqFunc eqFunc);

