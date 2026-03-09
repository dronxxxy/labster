#include "labster/core/utf8.hpp"
#include "labster/lexer/lexer.hpp"
#include "labster/lexer/token.hpp"
#include "labster/log/logger.hpp"
#include "labster/log/position.hpp"
#include "labster/log/stdLogger.hpp"
#include <memory>

int main(int argc, char **argv) {
    labster::FileContentRef content = std::make_shared<labster::FileContent>("примеры/главный.лс", labster::Utf8String(
        "привет, мир!\n"
        "как оно?"
    ));
    labster::LoggerRef logger = std::make_shared<labster::StdLogger>();

    auto lexer = labster::Lexer(logger, content);
    while (!lexer.next().isEoi());

    return 0;
}
