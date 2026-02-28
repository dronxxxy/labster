#pragma once

#include "labster/core/arena.h"
#include "labster/core/list.h"
#include "labster/core/slice.h"

typedef struct MemPool MemPool;

MemPool *mempoolNew();
void mempoolFree(MemPool *mempool);

Arena *mempoolArena(const MemPool *mempool);
void *mempoolAttachList(MemPool *mempool, void *list);
char *mempoolSliceToString(MemPool *mempool, Slice name);

#define mempoolListNew(mempool, type) mempoolAttachList(mempool, listNew(type))
#define mempoolListCreate(mempool, type, ...) mempoolAttachList(mempool, listCreate(type, ##__VA_ARGS__))
#define mempoolMapNew(mempool, type) mempoolAttachList(mempool, mapNew(type))
#define mempoolAlloc(mempool, size, alignment) arenaAlloc(mempoolArena(mempool), size, alignment)
#define mempoolAllocType(mempool, type) arenaAllocType(mempoolArena(mempool), type)
