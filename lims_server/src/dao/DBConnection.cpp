#include "dao/DBConnection.h"
#include "common/Config.h"
#include "common/Logger.h"
#include <Poco/Logger.h>

static std::string g_conn_str;

void DBConnection::init() {
    auto& config = Config::instance();
    // 构建PostgreSQL连接字符串
    g_conn_str = "host=" + config.getString("Database.host") +
                 " port=" + std::to_string(config.getInt("Database.port")) +
                 " dbname=" + config.getString("Database.dbname") +
                 " user=" + config.getString("Database.user") +
                 " password=" + config.getString("Database.password") +
                 " connect_timeout=" + std::to_string(config.getInt("Database.conn_timeout"));
    
    // 测试连接
    try {
        pqxx::connection test_conn(g_conn_str);
        if (test_conn.is_open()) {
            Logger::get().information("Database connection test success");
        }
    } catch (const std::exception& e) {
        Logger::get().error("Database connection test failed: %s", e.what());
        throw;
    }
}

std::unique_ptr<pqxx::connection> DBConnection::create() {
    try {
        auto conn = std::make_unique<pqxx::connection>(g_conn_str);
        if (!conn->is_open()) {
            throw std::runtime_error("Failed to open database connection");
        }
        return conn;
    } catch (const std::exception& e) {
        Logger::get().error("Database connection failed: %s", e.what());
        throw;
    }
}