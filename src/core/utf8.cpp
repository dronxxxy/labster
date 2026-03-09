#include "labster/core/utf8.hpp"

using namespace labster;

bool Utf8Char::operator==(char c) const {
  return this->value == c;  
}

bool Utf8Char::operator>=(char c) const {
  return this->value >= c;  
}

bool Utf8Char::operator<=(char c) const {
  return this->value <= c;  
}

bool Utf8Char::operator>(char c) const {
  return this->value > c;  
}

bool Utf8Char::operator<(char c) const {
  return this->value < c;  
}
  
Utf8StringIterator::Utf8StringIterator(std::string_view::const_iterator begin, std::string_view::const_iterator end) : position(begin), end(end) {
  readChar();
}

void Utf8StringIterator::readChar() {
  if (this->position >= this->end) return;

  uint8_t firstByte = static_cast<uint8_t>(*this->position);

  if ((firstByte & 0x80) == 0x00) this->length = 1;
  else if ((firstByte & 0xE0) == 0xC0) this->length = 2;
  else if ((firstByte & 0xF0) == 0xE0) this->length = 3;
  else if ((firstByte & 0xF8) == 0xF0) this->length = 4;
  else {
    this->length = 1; 
    this->result = { firstByte };
    return;
  }

  uint32_t result = 0;

  if (this->length == 1) result = firstByte;
  else if (this->length == 2) result = firstByte & 0x1F;
  else if (this->length == 3) result = firstByte & 0x0F;
  else if (this->length == 4) result = firstByte & 0x07;
  
  for (size_t i = 1; i < this->length; ++i) {
    uint8_t nextByte = static_cast<uint8_t>(*(this->position + i));
      
    if ((nextByte & 0xC0) != 0x80) {
      result = 0;
      break; 
    }

    result = (result << 6) | (nextByte & 0x3F);
  }
  this->result = { result };
}

bool Utf8StringIterator::operator!=(Utf8StringIterator other) const {
  return this->position != other.position;
}

Utf8StringIterator &Utf8StringIterator::operator++() {
  this->position += this->length;
  readChar();
  return *this;
}

Utf8Char Utf8StringIterator::operator*() const {
  return result;
}

std::string_view::const_iterator Utf8StringIterator::value() const {
  return this->position;
}

Utf8StringView::Utf8StringView(std::string_view view) : view(view) {}

Utf8StringIterator Utf8StringView::begin() const {
  return Utf8StringIterator(this->view.begin(), this->view.end());
}

Utf8StringIterator Utf8StringView::end() const {
  return Utf8StringIterator(this->view.end(), this->view.end());
}

Utf8String::Utf8String(std::string value) : string(value) {}

Utf8StringIterator Utf8String::begin() const {
  return Utf8StringView(this->string).begin();
}

Utf8StringIterator Utf8String::end() const {
  return Utf8StringView(this->string).end();
}

Utf8StringView Utf8String::view() const {
  return Utf8StringView(this->string);
}

std::ostream& labster::operator<<(std::ostream& os, labster::Utf8Char value) {
  uint32_t c = value.value;
  if (c <= 0x7F) {
    return os << static_cast<char>(c);
  } else if (c <= 0x7FF) {
    return os << static_cast<char>(0xC0 | (c >> 6))
              << static_cast<char>(0x80 | (c & 0x3F));
  } else if (c <= 0xFFFF) {
    return os << static_cast<char>(0xE0 | (c >> 12))
              << static_cast<char>(0x80 | ((c >> 6) & 0x3F))
              << static_cast<char>(0x80 | (c & 0x3F));
  } else {
    return os << static_cast<char>(0xF0 | (c >> 18))
              << static_cast<char>(0x80 | ((c >> 12) & 0x3F))
              << static_cast<char>(0x80 | ((c >> 6) & 0x3F))
              << static_cast<char>(0x80 | (c & 0x3F));
  }
  return os;
}
