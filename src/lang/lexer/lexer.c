#include "labster/lang/lexer/lexer.h"
#include "chars.h"
#include "labster/core/filePosition.h"
#include "labster/core/list.h"
#include "labster/core/logger.h"
#include "labster/core/memPool.h"
#include "labster/core/slice.h"
#include "labster/core/utf8.h"
#include "labster/lang/lexer/token.h"
#include <stdio.h>

typedef struct LangLexer {
  MemPool *mempool;
  Logger *logger;
  FileContent *content;
  size_t begin;
  size_t position;
} LangLexer;

LangLexer *langLexerNewIn(MemPool *mempool, Logger *logger, FileContent *content) {
   LangLexer *lexer = mempoolAllocType(mempool, LangLexer);
   lexer->mempool = mempool;
   lexer->logger = logger;
   lexer->content = content;
   lexer->begin = lexer->position = 0;
   return lexer;
}

static inline bool langLexerIsEoi(const LangLexer *lexer) {
  return lexer->position >= lexer->content->content.length;
}

static inline FilePosition langLexerGetPosition(const LangLexer *lexer) {
  return filePositionNew(lexer->content, lexer->begin, lexer->position);
}

static inline FilePosition langLexerGetCurrentPosition(const LangLexer *lexer) {
  return filePositionNew(lexer->content, lexer->position, lexer->position + 1);
}

static Utf8Char langLexerPeek(const LangLexer *lexer) {
  if (langLexerIsEoi(lexer)) return 0;
  size_t position = lexer->position;
  return utf8Next(lexer->content->content, &position);
}

static Utf8Char langLexerTake(LangLexer *lexer) {
  if (langLexerIsEoi(lexer)) return 0;
  return utf8Next(lexer->content->content, &lexer->position);
}

static void langLexerBegin(LangLexer *lexer) {
  lexer->begin = lexer->position;
}

static void langLexerSkipWhile(LangLexer *lexer, bool (*filter)(Utf8Char)) {
  while (!langLexerIsEoi(lexer) && filter(langLexerPeek(lexer))) langLexerTake(lexer);
}

static Utf8Char langLexerNextEscaped(LangLexer *lexer) {
  if (langLexerIsEoi(lexer)) {
    loggerLogAt(lexer->logger, langLexerGetCurrentPosition(lexer), LOGGER_ERROR, "an control sequence was expected");
    return 0;
  }
  switch (langLexerTake(lexer)) {
    case u'n': return '\n';
    case u't': return '\t';
    case u'r': return '\r';
    case u'0': return '\0';
    default:
      loggerLogAt(lexer->logger, langLexerGetCurrentPosition(lexer), LOGGER_ERROR, "an control sequence was expected");
      return 0;
  }
}

LangToken langLexerNext(LangLexer *lexer) {
  while (!langLexerIsEoi(lexer)) {
    langLexerSkipWhile(lexer, charIsWhitespace);
    langLexerBegin(lexer);
    if (langLexerIsEoi(lexer)) break;
    Utf8Char c = langLexerTake(lexer);
    switch (c) {
      case '(': return tokenNew(langLexerGetPosition(lexer), LANG_TOKEN_OPENING_CIRCLE_BRACE);
      case ')': return tokenNew(langLexerGetPosition(lexer), LANG_TOKEN_CLOSING_CIRCLE_BRACE);
      case '[': return tokenNew(langLexerGetPosition(lexer), LANG_TOKEN_OPENING_SQUARE_BRACE);
      case ']': return tokenNew(langLexerGetPosition(lexer), LANG_TOKEN_CLOSING_SQUARE_BRACE);
      case '{': return tokenNew(langLexerGetPosition(lexer), LANG_TOKEN_OPENING_FIGURE_BRACE);
      case '}': return tokenNew(langLexerGetPosition(lexer), LANG_TOKEN_CLOSING_FIGURE_BRACE);
      case ':': return tokenNew(langLexerGetPosition(lexer), LANG_TOKEN_COLON);
      case ',': return tokenNew(langLexerGetPosition(lexer), LANG_TOKEN_COMMA);
      case '.': return tokenNew(langLexerGetPosition(lexer), LANG_TOKEN_DOT);
      case '=': return tokenNew(langLexerGetPosition(lexer), LANG_TOKEN_ASSIGN);
      case '/':
        if (langLexerPeek(lexer) != '/') continue;
        break;
      case '"': {
        List(char) buffer = mempoolListNew(lexer->mempool, char);
        while ((c = langLexerTake(lexer)) != '\"') {
          if (langLexerIsEoi(lexer)) {
            loggerLogAt(lexer->logger, langLexerGetPosition(lexer), LOGGER_ERROR, "string literal was not finished");
            break;
          }
          if (c == '\\') listPush(buffer, langLexerNextEscaped(lexer));
          else listPush(buffer, c);
        }
        Slice string = sliceNew(buffer, listLength(buffer));
        return tokenNewString(langLexerGetPosition(lexer), string);
      }
      default:
        if (charIsIdentStart(c)) {
          langLexerSkipWhile(lexer, charIsIdentContinuation);
          FilePosition position = langLexerGetPosition(lexer);
          Slice slice = filePositionSlice(&position);
          if (sliceEq(slice, sliceFromString("fun"))) return tokenNew(position, LANG_TOKEN_FUN);
          return tokenNewIdent(position, slice);
        }
        if (charIsDigit(c)) {
          langLexerSkipWhile(lexer, charIsDigit);
          FilePosition position = langLexerGetPosition(lexer);
          Slice slice = filePositionSlice(&position);
          uint64_t result = 0;
          for (size_t i = 0; i < slice.length; i++)
            result = result * 10 + slice.pointer[i] - '0';
          return tokenNewInteger(position, result);
        }
        loggerLogAt(lexer->logger, langLexerGetPosition(lexer), LOGGER_ERROR, "unexpected symbol");
        break;
    }
  }
  return tokenNew(langLexerGetPosition(lexer), LANG_TOKEN_EOI);
}

