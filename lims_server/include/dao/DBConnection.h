#ifndef DB_CONNECTION_H
#define DB_CONNECTION_H

#include <pqxx/pqxx>
#include <memory>
#include "common/Config.h"

// 数据库连接池（简化版，单连接）
class DBConnection {
public:
    static std::shared_ptr<pqxx::connection> getConnection() {
        static std::shared_ptr<pqxx::connection> conn;
        if (!conn || !conn->is_open()) {
            std::string connStr = Config::getInstance().getDbConnectionString();
            conn = std::make_shared<pqxx::connection>(connStr);
        }
        return conn;
    }
};

#endif // DB_CONNECTION_H