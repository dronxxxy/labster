#include "labster/core/utf8.hpp"
#include "labster/log/logger.hpp"
#include "labster/log/position.hpp"
#include "labster/log/stdLogger.hpp"
#include <memory>
#include <string_view>

int main(int argc, char **argv) {
    labster::FileContentRef content = std::make_shared<labster::FileContent>("примеры/главный.лс", labster::Utf8String(
        "привет, мир!\n"
        "как оно?"
    ));
    auto position = labster::FilePosition(content, 8, 8);

    labster::LoggerRef logger = std::make_shared<labster::StdLogger>();
    logger->logAt(labster::LoggerLevel::LOGGER_ERROR, position, "Это ошибка");
    logger->log(labster::LoggerLevel::LOGGER_WARNING, "Это предупреждегние");
    logger->log(labster::LoggerLevel::LOGGER_INFO, "Это информация");
    return 0;
}
