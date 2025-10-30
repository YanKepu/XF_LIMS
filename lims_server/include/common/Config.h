#ifndef CONFIG_H
#define CONFIG_H

#include <Poco/Util/PropertyFileConfiguration.h>
#include <string>

class Config {
public:
    // 单例模式（全局唯一配置）
    static Config& getInstance() {
        static Config instance;
        return instance;
    }

    // 加载配置文件
    void load(const std::string& path) {
        config = Poco::Util::PropertyFileConfiguration::create(path);
    }

    // 获取服务器端口
    int getServerPort() const {
        return config->getInt("server.port", 8080); // 默认8080
    }

    // 获取数据库连接信息
    std::string getDbConnectionString() const {
        std::string dbname = config->getString("db.name", "lab_management");
        std::string user = config->getString("db.user", "postgres");
        std::string password = config->getString("db.password", "123456");
        std::string host = config->getString("db.host", "localhost");
        return "dbname=" + dbname + " user=" + user + " password=" + password + " host=" + host;
    }

private:
    Config() = default;
    Poco::AutoPtr<Poco::Util::PropertyFileConfiguration> config;
};

#endif // CONFIG_H