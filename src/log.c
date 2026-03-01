#include "labster/core/list.h"
#include "labster/core/logger.h"
#include <errno.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

static void logString(List(char) *output, va_list list) {
  const char *string = va_arg(list, const char *);
  listAppend(*output, string, strlen(string));
}

static void logSlice(List(char) *output, va_list list) {
  Slice slice = va_arg(list, Slice);
  listAppend(*output, slice.pointer, slice.length);
}

static void logSize(List(char) *output, va_list list) {
  size_t size = va_arg(list, size_t);
  char buffer[32];
  size_t length = snprintf(buffer, sizeof(buffer), "%lu", size);
  listAppend(*output, buffer, length);
}

static void logPercent(List(char) *output, va_list list) {
  listPush(*output, '%');
}

static void logError(List(char) *output, va_list list) {
  const char *string = strerror(errno);
  listAppend(*output, string, strlen(string));
}

__attribute__((constructor)) static void initLogger() {
  loggerRegisterKey(sliceFromString("%"), logPercent);
  loggerRegisterKey(sliceFromString("str"), logString);
  loggerRegisterKey(sliceFromString("slice"), logSlice);
  loggerRegisterKey(sliceFromString("errno"), logError);
  loggerRegisterKey(sliceFromString("size"), logSize);
}

