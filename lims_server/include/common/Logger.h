#ifndef LOGGER_H
#define LOGGER_H

#include <Poco/Logger.h>
#include <Poco/PatternFormatter.h>
#include <Poco/FormattingChannel.h>
#include <Poco/FileChannel.h>
#include <Poco/ConsoleChannel.h>
#include <Poco/SplitterChannel.h>
#include <Poco/AutoPtr.h>

namespace common 
{
    class Logger {
    public:
        void init();
        static Poco::Logger& getLogger() {
            static bool initialized = false;
            if (!initialized) {
                // 配置日志格式（时间 + 级别 + 消息）
                Poco::AutoPtr<Poco::PatternFormatter> formatter(new Poco::PatternFormatter("%Y-%m-%d %H:%M:%S [%p] %t"));
                Poco::AutoPtr<Poco::FormattingChannel> channel(new Poco::FormattingChannel(formatter));
                // 日志输出到文件（/var/log/lab_server/）
                Poco::AutoPtr<Poco::FileChannel> fileChannel(new Poco::FileChannel);
                fileChannel->setProperty("path", "/var/log/lab_server/server.log");
                fileChannel->setProperty("rotation", "daily"); // 按天轮转
                channel->setChannel(fileChannel);
                Poco::Logger::root().setChannel(channel);
                initialized = true;
            }
            return Poco::Logger::get("LabServer");
        }
    Poco::Logger& get();
    };
}


#endif // LOGGER_H