#pragma once

#include <stddef.h>

typedef struct Arena Arena;

Arena *arenaNew();
Arena *arenaNewWithCapacity(size_t capacity);

void *arenaAlloc(Arena *arena, size_t size, size_t alignment);

void arenaFree(Arena *arena);

#define arenaAllocType(arena, type) ((type*)arenaAlloc(arena, sizeof(type), _Alignof(type)))
