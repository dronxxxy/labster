#pragma once

#include "labster/core/utf8.h"
#include <stdbool.h>

static inline bool charIsLatin(Utf8Char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }

static inline bool charIsDigit(Utf8Char c) { return c >= '0' && c <= '9'; }
static inline bool charIsLetter(Utf8Char c) { return charIsLatin(c); }

static inline bool charIsIdentStart(Utf8Char c) { return charIsLetter(c); }
static inline bool charIsIdentContinuation(Utf8Char c) { return charIsIdentStart(c) || charIsDigit(c); }

static inline bool charIsWhitespace(Utf8Char c) { return c == '\n' || c == '\r' || c == ' ' || c == '\t'; }
static inline bool charIsNotNl(Utf8Char c) { return c != '\n'; }


