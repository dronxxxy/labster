#pragma once

#include <format>
#include <ostream>
#include <sstream>
#include <string_view>

namespace labster {
  struct Utf8Char {
    uint32_t value;

    bool operator==(char c) const;
    bool operator>=(char c) const;
    bool operator<=(char c) const;
    bool operator>(char c) const;
    bool operator<(char c) const;

    bool operator==(char16_t c) const;
    bool operator>=(char16_t c) const;
    bool operator<=(char16_t c) const;
    bool operator>(char16_t c) const;
    bool operator<(char16_t c) const;
  };
  
  class Utf8StringIterator {
    private:
      std::string_view::const_iterator position, end;
      Utf8Char result;
      size_t length;

      void readChar();

    public:
      explicit Utf8StringIterator(std::string_view::const_iterator begin, std::string_view::const_iterator end);

      bool operator!=(Utf8StringIterator other) const;
      Utf8StringIterator &operator++();
      Utf8Char operator*() const;

      std::string_view::const_iterator value() const;
  };

  class Utf8StringView {
    private:
      std::string_view view;
      
    public:    
      explicit Utf8StringView(std::string_view view);

      Utf8StringIterator begin() const;
      Utf8StringIterator end() const;
  };

  class Utf8String {
    private:
      std::string string;
      
    public:    
      explicit Utf8String(std::string value);

      Utf8StringView view() const;

      Utf8StringIterator begin() const;
      Utf8StringIterator end() const;
  };

  std::ostream& operator<<(std::ostream& os, labster::Utf8Char c);
}

template <>
struct std::formatter<labster::Utf8Char> {
  constexpr auto parse(std::format_parse_context& ctx) {
    auto it = ctx.begin();
    while (it != ctx.end() && *it != '}') {
      ++it;
    }
    return it;
  }

  auto format(const labster::Utf8Char& c, std::format_context& ctx) const {
    std::ostringstream buf;
    buf << c;
    return std::format_to(ctx.out(), "{}", buf.str());
  }
};
