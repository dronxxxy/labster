#pragma once

#include <assert.h>
#include <stddef.h>

static inline size_t alignedSize(size_t size, size_t alignment) {
  assert(alignment > 0);
  if (size % alignment == 0) return size;
  return size + alignment - size % alignment;
}

#define max(a, b) (a > b ? a : b)
#define min(a, b) (a < b ? a : b)
