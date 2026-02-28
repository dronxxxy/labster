#include "labster/core/utf8.h"
#include "labster/core/slice.h"
#include <assert.h>

uint8_t utf8GetMaskFromInitial(char c) {
  if ((c & 0b11111000) == 0b11110000) return 0b111;
  if ((c & 0b11110000) == 0b11100000) return 0b1111;
  if ((c & 0b11100000) == 0b11000000) return 0b11111;
  return 0b11111111;
}

uint8_t utf8GetSizeFromInitial(char c) {
  if ((c & 0b11111000) == 0b11110000) return 4;
  if ((c & 0b11110000) == 0b11100000) return 3;
  if ((c & 0b11100000) == 0b11000000) return 2;
  return 1;
}

Utf8Char utf8Next(Slice slice, size_t *position) {
  assert(*position < slice.length);

  uint8_t length = utf8GetSizeFromInitial(slice.pointer[*position]);
  uint8_t mask = utf8GetMaskFromInitial(slice.pointer[*position]);

  if (*position + length > slice.length) length = slice.length - *position;
  Utf8Char result = 0; 
  for (size_t i = 0; i < length; i++) {
    unsigned char c = slice.pointer[*position + i];
    c &= i == 0 ? mask : 0b111111;
    result = (result << 6) | c;
  }
  *position += length;
  return result;
}
