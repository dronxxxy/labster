#pragma once

#include "labster/core/utf8.hpp"
#include <memory>
#include <string_view>

namespace labster {
  class FileContent {
    public:
      const std::string_view filename;
      const Utf8String string;

      explicit FileContent(std::string_view filename, Utf8String string);
  };

  using FileContentRef = std::shared_ptr<FileContent>;

  struct FilePositionHighlight {
    std::string_view prefix, area, suffix;
  };

  class FilePosition {
    private:
      void resolvePosition();
      
    public:
      const FileContentRef content;
      size_t line, column;
      const size_t offset, length;

      explicit FilePosition(const FileContentRef content, size_t offset, size_t length);

      FilePositionHighlight getHighlight() const;
  };
}
