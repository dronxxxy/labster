#pragma once

#include "labster/core/list.h"
#include <stdint.h>
#include <string.h>

typedef List(char) String;

#define mempoolStringNew(mempool) mempoolListNew(mempool, char)
#define stringPush listPush
#define stringLength listLength
#define stringAppendSlice(string, slice) listAppend(string, (slice).pointer, (slice).length)
#define stringAppendCStr(string, cstr) listAppend(string, cstr, strlen(cstr))
#define stringAppendFmt(string, bufferSize, fmt, ...) ({ \
  char __buffer[128]; \
  size_t __length = snprintf(__buffer, sizeof(__buffer), fmt, ##__VA_ARGS__); \
  stringAppendSlice(string, sliceNew(__buffer, __length)); \
})
