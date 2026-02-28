#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef struct {
  const char *pointer;
  size_t length;
} Slice;

Slice sliceNew(const char *pointer, size_t length);
Slice sliceFromString(const char *string);
bool sliceEq(Slice a, Slice b);
bool sliceEqPtr(const Slice *a, const Slice *b);

Slice subSlice(Slice slice, size_t start, size_t end);
