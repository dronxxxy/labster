#pragma once

#include "labster/log/position.hpp"
#include <string_view>
#include <variant>

namespace labster {
  enum class SimpleToken {
    // Keywords
    TOKEN_IMPORT,
    TOKEN_FUNC,
    TOKEN_THEN,
    TOKEN_OBJECT,
    TOKEN_END,

    // Punctuation
    TOKEN_DOT, TOKEN_COMMA, TOKEN_COLON,
    TOKEN_OPENING_CIRCLE_BRACE, TOKEN_CLOSING_CIRCLE_BRACE,

    // Utility
    TOKEN_EOI,
  };

  struct IdentToken {
    std::string_view name;
  };
  
  using TokenContent = std::variant<IdentToken, SimpleToken>;

  class Token {
     public:
       const FilePosition position; 
       const TokenContent content;

       explicit Token(const FilePosition position, const TokenContent content);

       bool isSimple(SimpleToken token) const;
       bool isEoi() const;
  };
}
