#ifndef LOGGER_H
#define LOGGER_H

#include <Poco/Logger.h>
#include <Poco/PatternFormatter.h>
#include <Poco/FormattingChannel.h>
#include <Poco/FileChannel.h>
#include <Poco/ConsoleChannel.h>
#include <Poco/SplitterChannel.h>
#include <Poco/AutoPtr.h>
#include <Poco/Path.h>
#include <Poco/File.h>
#include <string>

namespace common 
{
    class Logger {
    public:
        void init();
        static Poco::Logger& getLogger();
        Poco::Logger& get();
    
    private:
        /*私有化构造函数，禁止外部实例化，仅能通过static方法使用 */
        Logger() = default;
        ~Logger() = default;

        // 禁止拷贝
        Logger(const Logger&) = default;
        Logger& operator=(const Logger&) = delete;
    };
}


#endif // LOGGER_H