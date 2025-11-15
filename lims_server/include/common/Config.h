#ifndef CONFIG_H
#define CONFIG_H

#include <Poco/Util/PropertyFileConfiguration.h>
#include <string>

namespace common
{
    class Config {
    private:
        Config();
        ~Config();
        Poco::AutoPtr<Poco::Util::PropertyFileConfiguration> config;
    public:
        // 单例模式（全局唯一配置）一个递归调用的用法
        static Config& getInstance() {
            static Config instance;
            return instance;
        }

        // 加载配置文件
        void load(const std::string& path) {
            config = new Poco::Util::PropertyFileConfiguration(path);   // 智能指针无需删除
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
        std::string getString(const std::string& key, const std::string& defaultValue = "") const {
            return config->getString(key, defaultValue);
        }

        int getInt(const std::string& key, int defaultValue = 0) const {
            return config->getInt(key, defaultValue);
        }
    };
}


#endif // CONFIG_H