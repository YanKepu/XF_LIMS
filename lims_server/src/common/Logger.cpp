#include "common/Logger.h"
#include "common/Config.h"


void common::Logger::init() {
    auto& config = Config::getInstance();
    
    // 格式化日志（时间+级别+消息）
    auto formatter = new Poco::PatternFormatter("%Y-%m-%d %H:%M:%S [%p] %t");
    formatter->setProperty("times", "local");

    // 输出通道（控制台+文件）
    auto splitter = new Poco::SplitterChannel();
    
    std::string logToConsole = config.getString("Log.console", "true");
    // 控制台输出
    if (logToConsole == "true") {
        auto console = new Poco::ConsoleChannel();
        splitter->addChannel(new Poco::FormattingChannel(formatter, console));
    }

    // 文件输出
    std::string logFile = config.getString("Log.file");
    if (!logFile.empty()) {
        auto file = new Poco::FileChannel();
        file->setProperty("path", logFile);
        file->setProperty("rotation", "size");
        file->setProperty("maxSize", std::to_string(config.getInt("Log.max_file_size")));
        file->setProperty("archive", "number");
        file->setProperty("keep", std::to_string(config.getInt("Log.max_files")));
        splitter->addChannel(new Poco::FormattingChannel(formatter, file));
    }

    // 设置日志级别
    std::string level = config.getString("Log.level", "info");
    Poco::Logger::root().setChannel(splitter);
    Poco::Logger::root().setLevel(
        level == "debug" ? Poco::Message::PRIO_DEBUG :
        level == "warn"  ? Poco::Message::PRIO_WARNING :
        level == "error" ? Poco::Message::PRIO_ERROR :
        Poco::Message::PRIO_INFORMATION
    );
}

Poco::Logger& common::Logger::get() {
    return Poco::Logger::get("LIMS");
}