#include "labster/core/stdLogger.h"
#include "labster/core/filePosition.h"
#include "labster/core/logger.h"
#include "labster/core/slice.h"
#include <stdio.h>

static inline const char *getPrefixFromLevel(LoggerLevel level) {
  switch (level) {
    case LOGGER_ERROR: return "ERROR";
    case LOGGER_WARNING: return "WARNING";
    case LOGGER_INFO: return "INFO";
  }
}

static inline FILE *getFileFromLevel(LoggerLevel level) {
  switch (level) {
    case LOGGER_ERROR: return stderr;
    case LOGGER_WARNING: return stderr;
    case LOGGER_INFO: return stdout;
  }
}

static inline int getAnsiColorFromLevel(LoggerLevel level) {
  switch (level) {
    case LOGGER_ERROR: return 31;
    case LOGGER_WARNING: return 33;
    case LOGGER_INFO: return 34;
  }
}

static inline void stdLog(LoggerLevel level, const char *message) {
  const char *prefix = getPrefixFromLevel(level);
  FILE *file = getFileFromLevel(level);
  int color = getAnsiColorFromLevel(level);
  fprintf(file, "\033[1;%dm%s:\033[0m %s\n", color, prefix, message);
}

static inline void stdLogAt(LoggerLevel level, FilePosition position, const char *message) {
  FILE *file = getFileFromLevel(level);
  stdLog(level, message);
  fprintf(file, "at file ");
  fwrite(position.content->name.pointer, 1, position.content->name.length, file);
  fprintf(file, ":%ld:\n", position.line);
  FileSelection selection = filePositionSelect(&position);
  fwrite(selection.before.pointer, 1, selection.before.length, file);
  fprintf(file, "\033[4m");
  fwrite(selection.highlight.pointer, 1, selection.highlight.length, file);
  fprintf(file, "\033[0m");
  fwrite(selection.after.pointer, 1, selection.after.length, file);
  fprintf(file, "\n");
}

Logger *stdLoggerNewAt(MemPool *mempool) {
  return loggerNewAt(mempool, stdLog, stdLogAt);
}

Logger *stdLoggerNew() {
  return loggerNew(stdLog, stdLogAt);
}

