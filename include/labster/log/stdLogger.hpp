#pragma once

#include "labster/log/logger.hpp"

namespace labster {
  class StdLogger final : public Logger {
    public:
      explicit StdLogger();

      void log(LoggerLevel level, std::string_view message) override;
      void logAt(LoggerLevel level, FilePosition position, std::string_view message) override;
  };
}
