#pragma once

#include "labster/core/memPool.h"
#include "labster/core/slice.h"
#include <stddef.h>

typedef struct {
  Slice name;
  Slice content;
} FileContent;

typedef struct Logger Logger;

FileContent *fileContentNewIn(MemPool *mempool, Slice name, Slice content);
FileContent *fileContentFromFile(MemPool *mempool, Logger *logger, const char *path);

typedef struct {
  FileContent *content;
  size_t begin, end;
  size_t line;
} FilePosition;

FilePosition filePositionNew(FileContent *content, size_t begin, size_t end);
Slice filePositionSlice(const FilePosition *position);
FilePosition filePositionUnion(FilePosition position, FilePosition other);

typedef struct {
  Slice before;
  Slice highlight;
  Slice after;
} FileSelection;

FileSelection filePositionSelect(const FilePosition *position);

