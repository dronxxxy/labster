#pragma once

#include "labster/core/filePosition.h"
#include "labster/core/list.h"
#include "labster/core/memPool.h"
#include "labster/core/slice.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>

typedef enum {
  LOGGER_ERROR,
  LOGGER_WARNING,
  LOGGER_INFO,
} LoggerLevel;

typedef void (*LoggerLogFn)(LoggerLevel level, const char *message);
typedef void (*LoggerLogAtFn)(LoggerLevel level, FilePosition position, const char *message);

typedef struct Logger Logger;

Logger *loggerNewAt(MemPool *mempool, LoggerLogFn logFn, LoggerLogAtFn logAtFn);
Logger *loggerNew(LoggerLogFn logFn, LoggerLogAtFn logAtFn);
void loggerFree(Logger *logger);

bool loggerWasError(const Logger *logger);

void loggerLogToV(Logger *logger, List(char) *output, const char *message, va_list list);
void loggerLogTo(Logger *logger, List(char) *output, const char *message, ...);
void loggerLogV(Logger *logger, LoggerLevel level, const char *message, va_list list);
void loggerLog(Logger *logger, LoggerLevel level, const char *message, ...);
void loggerLogAtV(Logger *logger, FilePosition position, LoggerLevel level, const char *message, va_list list);
void loggerLogAt(Logger *logger, FilePosition position, LoggerLevel level, const char *message, ...);

typedef void (*LoggerKeyFn)(List(char) *output, va_list list);
void loggerRegisterKey(Slice key, LoggerKeyFn fn);
