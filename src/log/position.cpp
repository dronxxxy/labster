#include "labster/log/position.hpp"
#include "labster/core/utf8.hpp"
#include <string_view>

using namespace labster;

FileContent::FileContent(std::string filename, Utf8String &&string) : filename(filename), string(std::move(string)) {}

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
  const char* rawData = this->content->string.begin().value();
  const char* rawEnd = this->content->string.end().value();
  
  const char* lineStart = rawData;
  const char* highlightBegin = rawEnd;
  const char* highlightEnd = rawEnd;
  const char* lineEnd = rawEnd;
  
  bool foundOffset = false;
  size_t i = 0;

  for (
    Utf8StringIterator iterator = this->content->string.begin();
    iterator != this->content->string.end();
    ++iterator, i++
  ) {
    const char* currentPtr = iterator.value();
    
    if (currentPtr >= rawEnd) break;

    if (i < this->offset) {
      if (*iterator == '\n') {
        const char* nextPtr = currentPtr + 1;
        lineStart = (nextPtr < rawEnd) ? nextPtr : rawEnd;
      }
      continue;
    }

    if (!foundOffset) {
      highlightBegin = currentPtr;
      foundOffset = true;
    }

    if (i == this->offset + this->length) highlightEnd = currentPtr;

    if (i >= this->offset + this->length) {
      if (*iterator == '\n') {
        lineEnd = currentPtr;
        break;
      }
      Utf8StringIterator nextIt = iterator;
      ++nextIt;
      if (nextIt != this->content->string.end()) lineEnd = nextIt.value();
    }
  }
  
  if (!foundOffset) {
    highlightBegin = rawEnd;
    highlightEnd = rawEnd;
  }
  
  if (highlightBegin > rawEnd) highlightBegin = rawEnd;
  if (highlightEnd > rawEnd) highlightEnd = rawEnd;
  if (lineEnd > rawEnd) lineEnd = rawEnd;
  if (lineStart > rawEnd) lineStart = rawEnd;

  return {
    std::string_view(lineStart, highlightBegin - lineStart),
    std::string_view(highlightBegin, highlightEnd - highlightBegin),
    std::string_view(highlightEnd, lineEnd - highlightEnd),
  };
}
