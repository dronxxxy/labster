#include "labster/core/logger.h"
#include "labster/core/filePosition.h"
#include "labster/core/list.h"
#include "labster/core/map.h"
#include "labster/core/memPool.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Logger {
  LoggerLogFn logFn;
  LoggerLogAtFn logAtFn;
  bool wasError;
  List(char) buffer;
} Logger;

Map(Slice, LoggerKeyFn) loggerKeys = NULL;

static inline void loggerInit(Logger *logger, LoggerLogFn logFn, LoggerLogAtFn logAtFn) {
  logger->logFn = logFn;
  logger->logAtFn = logAtFn;
  logger->wasError = false;
  logger->buffer = listNew(char);
}

Logger *loggerNewAt(MemPool *mempool, LoggerLogFn logFn, LoggerLogAtFn logAtFn) {
  Logger *logger = mempoolAllocType(mempool, Logger);
  loggerInit(logger, logFn, logAtFn);
  mempoolAttachList(mempool, logger->buffer);
  return logger;
}

Logger *loggerNew(LoggerLogFn logFn, LoggerLogAtFn logAtFn) {
  Logger *logger = malloc(sizeof(Logger));
  loggerInit(logger, logFn, logAtFn);
  return logger;
}

void loggerFree(Logger *logger) {
  free(logger);
}

bool loggerWasError(const Logger *logger) {
  return logger->wasError;
}

static const char *loggerBuildString(Logger *logger, const char *message, va_list list) {
  listClear(logger->buffer);
  loggerLogToV(logger, &logger->buffer, message, list);
  listPush(logger->buffer, 0);
  return logger->buffer;
}

static inline void loggerHandleLevel(Logger *logger, LoggerLevel level) {
  if (level == LOGGER_ERROR) logger->wasError = true;
}

void loggerLogToV(Logger *logger, List(char) *output, const char *message, va_list list) {
  for (size_t i = 0; message[i] != '\0'; i++) {
    bool wasKey = false;
    if (loggerKeys != NULL && message[i] == '%') {
      mapFor (pair, loggerKeys) {
        if (!strncmp(&message[i + 1], pair->key.pointer, pair->key.length)) {
          pair->value(output, list);
          i += pair->key.length;
          wasKey = true;
          break;
        }
      }
    }
    if (!wasKey) listPush(logger->buffer, message[i]);
  }
}

void loggerLogTo(Logger *logger, List(char) *output, const char *message, ...) {
  va_list list;
  va_start(list, message);
  loggerLogToV(logger, output, message, list);
  va_end(list);
}

void loggerLogAtV(Logger *logger, FilePosition position, LoggerLevel level, const char *message, va_list list) {
  loggerHandleLevel(logger, level);
  logger->logAtFn(level, position, loggerBuildString(logger, message, list));
}

void loggerLogV(Logger *logger, LoggerLevel level, const char *message, va_list list) {
  loggerHandleLevel(logger, level);
  logger->logFn(level, loggerBuildString(logger, message, list));
}

void loggerLogAt(Logger *logger, FilePosition position, LoggerLevel level, const char *message, ...) {
  va_list list;
  va_start(list, message);
  loggerLogAtV(logger, position, level, message, list);
  va_end(list);
}

void loggerLog(Logger *logger, LoggerLevel level, const char *message, ...) {
  va_list list;
  va_start(list, message);
  loggerLogV(logger, level, message, list);
  va_end(list);
}

void loggerRegisterKey(Slice key, LoggerKeyFn fn) {
  if (!loggerKeys) loggerKeys = mapNew(typeof(loggerKeys));
  mapInsert(loggerKeys, key, fn);
}

__attribute__((destructor)) static void loggerKill() {
  if (loggerKeys) mapFree(loggerKeys);
  loggerKeys = NULL;
}
