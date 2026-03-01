#pragma once

#include "labster/core/list.h"
typedef struct AstExpression AstExpression;

typedef enum {
  AST_STATEMENT_EXPRESSION,
} AstStatementKind;

typedef struct {
  AstStatementKind kind;

  union {
    AstExpression *expression;    
  };
} AstStatement;

typedef struct AstBody {
  List(AstStatement) statements;
} AstBody;
