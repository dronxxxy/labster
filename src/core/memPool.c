#include "labster/core/memPool.h"
#include "labster/core/arena.h"
#include "labster/core/list.h"
#include <string.h>

typedef struct MemPool {
  Arena *arena;
  void **links;
} MemPool;

MemPool *mempoolNew() {
  Arena *arena = arenaNew();
  MemPool *mempool = arenaAllocType(arena, MemPool);
  mempool->arena = arena;
  mempool->links = listNew(void*);
  return mempool;
}

void mempoolFree(MemPool *mempool) {
  listFor (link, mempool->links)
    if (link)
      listFree(link);
  listFree(mempool->links);
  arenaFree(mempool->arena);
}

Arena *mempoolArena(const MemPool *mempool) {
  return mempool->arena;
}

void *mempoolAttachList(MemPool *mempool, void *list) {
  void *oldList = mempool->links;
  listPush(mempool->links, list);
  listLink(list, &listLast(mempool->links));

  if (oldList != mempool->links) {
    listForPtr(link, mempool->links)
      if (*link)
        listLink(*link, link);
  }
  return list;
}

char *mempoolSliceToString(MemPool *mempool, Slice slice) {
  char *result = mempoolAlloc(mempool, slice.length + 1, 1);
  memcpy(result, slice.pointer, slice.length);
  result[slice.length] = '\0';
  return result;
}
