#include "labster/core/slice.h"
#include <assert.h>
#include <string.h>

Slice sliceNew(const char *pointer, size_t length) {
  return (Slice) {
    .pointer = pointer,
    .length = length,
  };
}

Slice sliceFromString(const char *string) {
  return sliceNew(string, strlen(string));
}

bool sliceEqPtr(const Slice *a, const Slice *b) {
  if (a->length != b->length) return false; 
  return !memcmp(a->pointer, b->pointer, a->length);
}

bool sliceEq(Slice a, Slice b) {
  return sliceEqPtr(&a, &b);
}

Slice subSlice(Slice slice, size_t start, size_t end) {
  assert(start <= end);
  assert(slice.length >= end);
  slice.pointer += start;
  slice.length = end - start;
  return slice;
}
