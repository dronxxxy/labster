#pragma once

#include "position.hpp"
#include <memory>
#include <string_view>

namespace labster {
  enum class LoggerLevel {
    LOGGER_ERROR,
    LOGGER_WARNING,
    LOGGER_INFO,
  };
  
  class Logger {
    public:
      virtual void log(LoggerLevel level, std::string_view message) = 0;
      virtual void logAt(LoggerLevel level, FilePosition position, std::string_view message) = 0;
  };

  using LoggerRef = std::shared_ptr<Logger>;
}
