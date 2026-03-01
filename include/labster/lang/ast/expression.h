#pragma once

#include "labster/core/list.h"
#include "labster/core/memPool.h"
#include "labster/core/slice.h"

typedef enum {
  AST_EXPRESSION_RESOLVE,
  AST_EXPRESSION_MEMBER,
  AST_EXPRESSION_CALL,
} AstExpressionKind;

typedef struct AstExpression AstExpression;

typedef struct {
  bool isNamed;
  Slice name;
  AstExpression *value;
} AstExpressionCallArgument;

typedef struct AstExpression {
  AstExpressionKind kind;

  union {
    struct {
      Slice name;
    } resolve;

    struct {
      AstExpression *callable;
      List(AstExpressionCallArgument) arguments;
    } call;

    struct {
      AstExpression *inner;
      Slice name;
    } member;
  };
} AstExpression;

static inline AstExpressionCallArgument astExpressionCallArgumentNewPositional(AstExpression *value) {
  return (AstExpressionCallArgument) { .isNamed = false, .value = value };
}

static inline AstExpressionCallArgument astExpressionCallArgumentNewNamed(Slice name, AstExpression *value) {
  return (AstExpressionCallArgument) { .isNamed = true, .name = name, .value = value };
}

AstExpression *astExpressioNewResolve(MemPool *mempool, Slice name);
AstExpression *astExpressioNewMember(MemPool *mempool, AstExpression *inner, Slice name);
AstExpression *astExpressioNewCall(MemPool *mempool, AstExpression *callable, List(AstExpressionCallArgument) arguments);
