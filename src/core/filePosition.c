#include "labster/core/filePosition.h"
#include "labster/core/logger.h"
#include "labster/core/memPool.h"
#include "labster/core/slice.h"
#include "labster/core/utf8.h"
#include "labster/core/utils.h"
#include <assert.h>
#include <stdio.h>

FileContent *fileContentNewIn(MemPool *mempool, Slice name, Slice content) {
  FileContent *result = mempoolAllocType(mempool, FileContent);
  result->name = name;
  result->content = content;
  return result;
}

FileContent *fileContentFromFile(MemPool *mempool, Logger *logger, const char *path) {
  FILE *file = fopen(path, "r");
  if (!file) {
    loggerLog(logger, LOGGER_ERROR, "failed to open the file: %errno");
    return NULL;
  }
  fseek(file, 0, SEEK_END);
  size_t length = ftell(file);
  fseek(file, 0, SEEK_SET);
  char *buffer = mempoolAlloc(mempool, length, 1);
  fread(buffer, 1, length, file);
  fclose(file);
  Slice content = sliceNew(buffer, length);
  return fileContentNewIn(mempool, sliceFromString(path), content);
}

FilePosition filePositionNew(FileContent *content, size_t begin, size_t end) {
  size_t line = 0;
  for (size_t i = 0; i < begin;) {
    Utf8Char c;
    while (i < begin && (c = utf8Next(content->content, &i)) != '\n');
    if (c == '\n') {
      i++;
      line++;
    }
  }
  return (FilePosition) {
    .content = content,
    .begin = begin,
    .end = end,
    .line = line,
  };
}

Slice filePositionSlice(const FilePosition *position) {
  return subSlice(position->content->content, position->begin, position->end);
}

FilePosition filePositionUnion(FilePosition position, FilePosition other) {
  assert(position.content == other.content);
  position.begin = min(position.begin, other.begin);
  position.end = max(position.end, other.end);
  return position;
}

FileSelection filePositionSelect(const FilePosition *position) {
  size_t start = 0;
  size_t end = 0;
  for (size_t i = 0; i < position->content->content.length;) {
    if (i <= position->begin) start = i;
    Utf8Char c;
    while (i < position->content->content.length && (c = utf8Next(position->content->content, &i)) != '\n');
    if (i >= position->end) {
      end = c == '\n' ? i - 1 : i;
      break;
    }
  }
  assert(start <= position->begin);
  assert(end >= position->end);
  return (FileSelection) {
    .before = subSlice(position->content->content, start, position->begin),
    .highlight = subSlice(position->content->content, position->begin, position->end),
    .after = subSlice(position->content->content, position->end, end),
  };
}
