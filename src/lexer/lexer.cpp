#include "labster/lexer/lexer.hpp"
#include "labster/core/utf8.hpp"
#include "labster/lexer/token.hpp"
#include "labster/log/logger.hpp"
#include "labster/log/position.hpp"
#include <string_view>

using namespace labster;

Lexer::Lexer(LoggerRef logger, FileContentRef content) :
  logger(logger), content(content), iterator(content->string.begin()), offset(0), beginOffset(0) {}

Utf8Char Lexer::peekChar() {
  return *this->iterator;
}

Utf8Char Lexer::takeChar() {
  if (this->isFinished()) return { 0 };
  this->offset++;
  auto value = *this->iterator;
  ++this->iterator;
  return value;
}

void Lexer::skipWhile(std::function<bool (Utf8Char)> filter) {
  while (!this->isFinished() && filter(this->peekChar())) this->takeChar();
}

bool Lexer::isFinished() const {
  return this->iterator.value() >= this->content->string.end().value();
}

void Lexer::begin() {
  this->beginOffset = this->offset;
}

FilePosition Lexer::position() const {
  return FilePosition(this->content, this->beginOffset, this->offset - this->beginOffset);
}

Token Lexer::buildToken(TokenContent content) {
  return Token(this->position(), content);
}

static bool utfCharIsIdentStart(Utf8Char c) {
  return
    (c >= u'а' && c <= u'я') ||
    (c >= u'А' && c <= u'Я') ||
    c == '_';
}

static bool utfCharIsIdentContinuation(Utf8Char c) {
  return utfCharIsIdentStart(c) || (c >= '0' && c <= '9');
}

Token Lexer::next() {
  this->skipWhile([](Utf8Char c) { return c == ' ' || c == '\t' || c == '\n'; });
  this->begin();

  if (this->isFinished()) return this->buildToken(SimpleToken::TOKEN_EOI);
  
  auto c = this->takeChar();
  if (c == '.') return this->buildToken(SimpleToken::TOKEN_DOT);
  if (c == ',') return this->buildToken(SimpleToken::TOKEN_COMMA);
  if (c == ':') return this->buildToken(SimpleToken::TOKEN_COLON);
  if (c == '(') return this->buildToken(SimpleToken::TOKEN_OPENING_CIRCLE_BRACE);
  if (c == ')') return this->buildToken(SimpleToken::TOKEN_CLOSING_CIRCLE_BRACE);

  if (utfCharIsIdentStart(c)) {
    auto position = this->iterator;
    this->skipWhile(utfCharIsIdentContinuation);
    auto end = this->iterator;
    return this->buildToken(IdentToken(std::string_view(position.value(), end.value())));
  }

  this->logger->logAt(LoggerLevel::LOGGER_ERROR, this->position(), std::format("не удается распознать токен"));
  return this->next();
}

