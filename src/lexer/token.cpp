#include "labster/lexer/token.hpp"
#include <variant>

using namespace labster;

Token::Token(const FilePosition position, const TokenContent content) :
  position(position), content(content) {}

bool Token::isSimple(SimpleToken token) const {
  auto current = std::get_if<SimpleToken>(&this->content);
  return current && *current == token;
}

bool Token::isEoi() const {
  return this->isSimple(SimpleToken::TOKEN_EOI);
}
