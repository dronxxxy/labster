#include "labster/core/memPool.h"
#include "labster/lang/ast/expression.h"

static LangAstExpression *astExpressionNew(MemPool *mempool, AstExpressionKind kind) {
  LangAstExpression *expression = mempoolAllocType(mempool, LangAstExpression);
  expression->kind = kind;
  return expression;
}

LangAstExpression *astExpressioNewResolve(MemPool *mempool, Slice name) {
  LangAstExpression *expression = astExpressionNew(mempool, LANG_AST_EXPRESSION_RESOLVE);
  expression->resolve.name = name;
  return expression;
}

LangAstExpression *astExpressioNewMember(MemPool *mempool, LangAstExpression *inner, Slice name) {
  LangAstExpression *expression = astExpressionNew(mempool, LANG_AST_EXPRESSION_MEMBER);
  expression->member.inner = inner;
  expression->member.name = name;
  return expression;
}

LangAstExpression *astExpressioNewCall(MemPool *mempool, LangAstExpression *callable, List(LangAstExpressionCallArgument) arguments) {
  LangAstExpression *expression = astExpressionNew(mempool, LANG_AST_EXPRESSION_CALL);
  expression->call.callable = callable;
  expression->call.arguments = arguments;
  return expression;
}

