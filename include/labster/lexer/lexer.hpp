#pragma once

#include "labster/core/utf8.hpp"
#include "labster/lexer/token.hpp"
#include "labster/log/logger.hpp"
#include "labster/log/position.hpp"
#include <functional>

namespace labster {
  class Lexer {
    private:
      LoggerRef logger;
      FileContentRef content;
      Utf8StringIterator iterator;
      size_t offset;
      size_t beginOffset;

      Utf8Char peekChar();
      Utf8Char takeChar();
      void skipWhile(std::function<bool (Utf8Char)> filter);

      bool isFinished() const;

      void begin();
      FilePosition position() const;
      Token buildToken(TokenContent content);
      
    public:
      explicit Lexer(LoggerRef logger, FileContentRef content);

      Token next();
  };
}
