#pragma once

#include "labster/core/filePosition.h"
#include "labster/core/logger.h"
#include "labster/lang/lexer/token.h"

typedef struct LangLexer LangLexer;

LangLexer *langLexerNewIn(MemPool *mempool, Logger *logger, FileContent *content);
LangToken langLexerNext(LangLexer *lexer);
