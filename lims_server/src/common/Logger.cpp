#include "common/Logger.h"
#include "common/Config.h"


/*
调用 logger.information("消息") 
  ↓
创建 Poco::Message 对象（包含级别、内容、时间戳）
  ↓
交给 FormattingChannel 处理 → 用 PatternFormatter 格式化（按 %Y-%m-%d... 拼接）
  ↓
交给 FileChannel 处理 → 写入 /var/log/lab_server/server.log 文件
  ↓
若开启轮转（daily），FileChannel 会自动管理文件（按天切分、清理旧日志）
*/

void common::Logger::init() {
    try
    {
        /* 获取config中定义的日志级别 */
        common::Config& config = common::Config::getInstance();

        /* 1. 定义日志路径 */
        const std::string logFilePath = config.getString("log.file", "/home/ubuntu/XF_LIMS/lims_server.log");

        /* 2. 解析出上级目录路径 */
        Poco::Path logPath(logFilePath);
        Poco::File logDir(logPath.parent());

        /* 3. 自动创建目录（递归创建） */
        if(!logDir.exists()){
            logDir.createDirectories();     // 递归创建所有上级目录
        }

        /* 4. 配置日志格式和通道 */
        // 格式化日志（时间+级别+消息）
        Poco::AutoPtr<Poco::PatternFormatter> formatter(
            new Poco::PatternFormatter("%Y-%m-%d %H:%M:%S [%p] %t")
        );
        Poco::AutoPtr<Poco::FormattingChannel> channel(
            new Poco::FormattingChannel(formatter)
        );
        Poco::AutoPtr<Poco::FileChannel> fileChannel(new Poco::FileChannel);
        fileChannel->setProperty("path", logFilePath);
        fileChannel->setProperty("rotation", "daily");  // 按天轮转
        fileChannel->setProperty("archive", "timestamp");   // 归档文件增加时间戳
        fileChannel->setProperty("purgeAge", "30");     // 保留30天日志

        /* 5. 设置根日志通道和级别*/
        Poco::Logger::root().setChannel(channel);

        std::string level = config.getString("Log.level", "info");
        if (level == "info"){
            Poco::Logger::root().setLevel(Poco::Message::PRIO_INFORMATION);
        }
        if (level == "debug"){
            Poco::Logger::root().setLevel(Poco::Message::PRIO_DEBUG);
        }
        else if (level == "warn") {
            Poco::Logger::root().setLevel(Poco::Message::PRIO_WARNING);
        }
        else if (level == "error"){
            Poco::Logger::root().setLevel(Poco::Message::PRIO_ERROR);
        }
    }
    catch(Poco::Exception& e)
    {
        std::cerr << "Logger初始化失败：" << e.displayText() << std::endl;
    }

}

Poco::Logger& common::Logger::getLogger() {
            static bool initialized = false;
            if (!initialized) {
                // 配置日志格式（时间 + 级别 + 消息）
                Poco::AutoPtr<Poco::PatternFormatter> formatter(new Poco::PatternFormatter("%Y-%m-%d %H:%M:%S [%p] %t"));
                Poco::AutoPtr<Poco::FormattingChannel> channel(new Poco::FormattingChannel(formatter));
                // 日志输出到文件（/var/log/lab_server/）
                Poco::AutoPtr<Poco::FileChannel> fileChannel(new Poco::FileChannel);
                fileChannel->setProperty("path", "/home/ubuntu/XF_LIMS/lims_server/server.log");
                fileChannel->setProperty("rotation", "daily"); // 按天轮转
                channel->setChannel(fileChannel);
                Poco::Logger::root().setChannel(channel);
                initialized = true;
            }
            return Poco::Logger::get("LabServer");
        }

Poco::Logger& common::Logger::get() {
    return getLogger();
}