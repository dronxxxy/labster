#pragma once

#include "labster/core/filePosition.h"
#include "labster/core/slice.h"
#include <stdint.h>

typedef enum {
  // Keywords
  LANG_TOKEN_FUN,

  // Punctuation
  LANG_TOKEN_COMMA, LANG_TOKEN_COLON, LANG_TOKEN_DOT,
  LANG_TOKEN_OPENING_CIRCLE_BRACE, LANG_TOKEN_CLOSING_CIRCLE_BRACE,
  LANG_TOKEN_OPENING_SQUARE_BRACE, LANG_TOKEN_CLOSING_SQUARE_BRACE,
  LANG_TOKEN_OPENING_FIGURE_BRACE, LANG_TOKEN_CLOSING_FIGURE_BRACE,

  // Operators
  LANG_TOKEN_ASSIGN,

  // Complex
  LANG_TOKEN_IDENT, LANG_TOKEN_INTEGER, LANG_TOKEN_STRING,

  // Utility
  LANG_TOKEN_EOI, LANG_TOKEN_FAILED,
} LangTokenKind;

typedef struct {
  LangTokenKind kind;
  FilePosition position;

  union {
    Slice ident, string;   
    uint64_t integer;
  };
} LangToken;

static inline LangToken tokenNew(FilePosition position, LangTokenKind kind) {
  return (LangToken) {
    .kind = kind,
    .position = position,
  }; 
}

static inline LangToken tokenNewIdent(FilePosition position, Slice ident) {
  return (LangToken) {
    .kind = LANG_TOKEN_IDENT,
    .position = position,
    .ident = ident,
  }; 
}

static inline LangToken tokenNewString(FilePosition position, Slice string) {
  return (LangToken) {
    .kind = LANG_TOKEN_STRING,
    .position = position,
    .string = string,
  }; 
}

static inline LangToken tokenNewInteger(FilePosition position, uint64_t integer) {
  return (LangToken) {
    .kind = LANG_TOKEN_INTEGER,
    .position = position,
    .integer = integer,
  }; 
}

