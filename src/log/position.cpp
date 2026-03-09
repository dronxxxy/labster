#include "labster/log/position.hpp"
#include "labster/core/utf8.hpp"
#include <string_view>

using namespace labster;

FileContent::FileContent(std::string_view filename, Utf8String string) : filename(filename), string(string) {}

FilePosition::FilePosition(const FileContentRef content, size_t offset, size_t length) :
  content(content), offset(offset), length(length) {
    this->resolvePosition();    
  }

void FilePosition::resolvePosition() {
  this->line = 1;
  this->column = 0;
  size_t i = 0;
  for (auto c : this->content->string) {
    if (i++ >= this->offset) break;
    if (c == '\n') {
      this->line++;
      this->column = 0;
    }
    this->column++;
  }
}

FilePositionHighlight FilePosition::getHighlight() const {
  size_t i = 0;
  std::string_view::const_iterator
    lineStart = this->content->string.begin().value(),
    highlightBegin, highlightEnd, lineEnd;
  bool lineEnded = false;
  for (
    Utf8StringIterator iterator = this->content->string.begin();
    iterator != ++this->content->string.end();
    ++iterator, i++
  ) {
    if (i < this->offset) {
      if (*iterator == '\n') lineStart = iterator.value();
      continue;
    }
    if (i == this->offset) highlightBegin = iterator.value();
    if (i <= this->offset + this->length) highlightEnd = iterator.value();
    if (i >= this->offset + this->length && *iterator == '\n') {
      lineEnded = true;
      break;
    }
    lineEnd = iterator.value();
  }
  if (!lineEnded) lineEnd = this->content->string.end().value();

  return {
    std::string_view(lineStart, highlightBegin),
    std::string_view(highlightBegin, highlightEnd),
    std::string_view(highlightEnd, lineEnd),
  };
}

