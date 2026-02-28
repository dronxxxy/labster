#include "labster/core/arena.h"
#include "labster/core/utils.h"
#include <assert.h>
#include <stdlib.h>

typedef struct Arena {
  size_t capacity;
  size_t top;
  Arena *next;

  char data[];
} Arena;

Arena *arenaNew() {
  return arenaNewWithCapacity(512);
}

static inline Arena *arenaNewWithCapacityAligned(size_t capacity, size_t alignment) {
  size_t size = alignedSize(sizeof(Arena), alignment);
  Arena *arena = malloc(size + capacity);

  size_t padding = size - sizeof(Arena);
  arena->capacity = capacity + padding;
  arena->top = 0;
  arena->next = NULL;

  return arena;
}

Arena *arenaNewWithCapacity(size_t capacity) {
  return arenaNewWithCapacityAligned(capacity, 1);
}

static inline size_t arenaRemains(const Arena *arena, size_t alignment) {
  size_t alignedTop = alignedSize(arena->top, alignment);
  if (alignedTop > arena->capacity) return 0;
  return arena->capacity - alignedTop;
}

void *arenaAlloc(Arena *arena, size_t size, size_t alignment) {
  while (arenaRemains(arena, alignment) < size) {
    if (arena->next) {
      arena = arena->next;
      continue;
    }
    arena->next = arenaNewWithCapacityAligned(max(arena->capacity, size), alignment);
    void *result = arenaAlloc(arena->next, size, alignment);
    assert(result);
    return result;
  }
  size_t alignedTop = alignedSize(arena->top, alignment);
  void *result = arena->data + alignedTop;

  arena->top = alignedTop + size;
  assert(arena->top <= arena->capacity);

  return result;
}

void arenaFree(Arena *arena) {
  if (arena->next) arenaFree(arena->next);
  free(arena);
}

