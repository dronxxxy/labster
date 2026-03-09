#include "labster/log/stdLogger.hpp"
#include "labster/log/logger.hpp"
#include <iostream>
#include <ostream>
#include <string_view>
#include <utility>

#define ANSI_SEQUENCE(id) "\033[" id "m"
#define ANSI_RESET ANSI_SEQUENCE("0")

using namespace labster;

StdLogger::StdLogger() {}

static std::string_view logLevelToAnsiColor(LoggerLevel level) {
  switch (level) {
    case LoggerLevel::LOGGER_ERROR: return ANSI_SEQUENCE("31;1");
    case LoggerLevel::LOGGER_WARNING: return ANSI_SEQUENCE("33;1");
    case LoggerLevel::LOGGER_INFO: return ANSI_SEQUENCE("34;1");
  }
  std::unreachable();
}

static std::string_view logLevelToAnsiHighlightColor(LoggerLevel level) {
  switch (level) {
    case LoggerLevel::LOGGER_ERROR: return ANSI_SEQUENCE("31;4");
    case LoggerLevel::LOGGER_WARNING: return ANSI_SEQUENCE("33;4");
    case LoggerLevel::LOGGER_INFO: return ANSI_SEQUENCE("34;4");
  }
  std::unreachable();
}

static std::string_view logLevelToPrefix(LoggerLevel level) {
  switch (level) {
    case LoggerLevel::LOGGER_ERROR: return "ОШИБКА";
    case LoggerLevel::LOGGER_WARNING: return "ПРЕДУПРЕЖДЕНИЕ";
    case LoggerLevel::LOGGER_INFO: return "ИНФОРМАЦИЯ";
  }
  std::unreachable();
}

static std::ostream &logLevelToStream(LoggerLevel level) {
  switch (level) {
    case LoggerLevel::LOGGER_ERROR: return std::cout;
    case LoggerLevel::LOGGER_WARNING: return std::cerr;
    case LoggerLevel::LOGGER_INFO: return std::cerr;
  }
  std::unreachable();
}

void StdLogger::log(LoggerLevel level, std::string_view message) {
  logLevelToStream(level) <<
    logLevelToAnsiColor(level) << logLevelToPrefix(level) << ": " << ANSI_RESET <<
    message << std::endl;
}

static std::string padOfLength(size_t length) {
  std::string result = "";
  for (size_t i = 0; i < length; i++) result += " ";
  return result;
}

static void writeStringWithOffset(std::ostream &os, bool isStart, size_t line, std::string_view ansiSequence, std::string_view view) {
  const std::string_view LineBeginPattern = " | ";

  std::string lineStr = std::to_string(line);
  std::string lineBegin = std::string("   ") +
                          (isStart ? std::to_string(line) : padOfLength(lineStr.size())) +
                          std::string(LineBeginPattern);

  if (isStart) std::cout << ANSI_RESET << lineBegin;
  std::cout << ansiSequence;
  for (auto c : view) {
    if (c == '\n') {
      std::cout << '\n' << ANSI_RESET << lineBegin << ansiSequence;
      continue;
    }
    std::cout << c;
  }
  std::cout << ANSI_RESET;
}

void StdLogger::logAt(LoggerLevel level, FilePosition position, std::string_view message) {
  this->log(level, message);
  auto &stream = logLevelToStream(level);

  stream
    << "> в файле " ANSI_SEQUENCE("4;36") << position.content->filename
    << ":" << position.line
    << ":" << position.column
    << ANSI_RESET ":"
    << std::endl;

  auto highlight = position.getHighlight();
  writeStringWithOffset(stream, true, position.line, "", highlight.prefix);
  writeStringWithOffset(stream, false, position.line, logLevelToAnsiHighlightColor(level), highlight.area);
  writeStringWithOffset(stream, false, position.line, "", highlight.suffix);
  stream << std::endl << std::endl;
}
