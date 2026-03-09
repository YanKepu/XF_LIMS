#include "dao/UserDAO.h"
#include "common/Logger.h"
#include <pqxx/transaction>

bool UserDAO::existsByUsername(const std::string& username) {
    std::shared_ptr<pqxx::connection> dbConn = nullptr;
    try {
        dbConn = DBConnection::getConnection(); // 复用你的DBConnection获取连接
        pqxx::work tx(*dbConn);
        std::string sql = "SELECT 1 FROM \"user\" WHERE username = $1 LIMIT 1"; // 注意user是关键字，加引号
        pqxx::result res = tx.exec_params(sql, username);
        tx.commit();
        return !res.empty();
    } catch (const pqxx::sql_error& e) {
        common::Logger::getLogger().error("UserDAO existsByUsername error: " + std::string(e.what()));
        throw; // 抛给上层Service处理
    }
}

void UserDAO::insertUser(const User& user) {
    std::shared_ptr<pqxx::connection> dbConn = nullptr;
    try {
        dbConn = DBConnection::getConnection();
        pqxx::work tx(*dbConn);
        std::string sql = R"(
            INSERT INTO "user" (username, password, phone, status, create_time)
            VALUES ($1, $2, $3, $4, $5)
        )";
        tx.exec_params(
            sql,
            user.getUsername(),
            user.getPassword(),
            user.getPhone(),
            user.getStatus(),
            user.getCreateTime()
        );
        tx.commit();
        common::Logger::getLogger().information("UserDAO insertUser success: " + user.getUsername());
    } catch (const pqxx::sql_error& e) {
        common::Logger::getLogger().error("UserDAO insertUser error: " + std::string(e.what()));
        throw;
    }
}

User UserDAO::findByUsername(const std::string& username) {
    std::shared_ptr<pqxx::connection> dbConn = nullptr;
    try {
        dbConn = DBConnection::getConnection();
        pqxx::work tx(*dbConn);
        std::string sql = R"(
            SELECT id, username, password, phone, status, create_time
            FROM "user" WHERE username = $1
        )";
        pqxx::result res = tx.exec_params(sql, username);
        tx.commit();

        if (res.empty()) {
            common::Logger::getLogger().warning("UserDAO findByUsername not found: " + username);
            throw std::runtime_error("user not found");
        }

        // 映射到User对象
        User user;
        user.setId(res[0]["id"].as<int>());
        user.setUsername(res[0]["username"].as<std::string>());
        user.setPassword(res[0]["password"].as<std::string>());
        user.setPhone(res[0]["phone"].as<std::string>());
        user.setStatus(res[0]["status"].as<int>());
        user.setCreateTime(res[0]["create_time"].as<time_t>());
        return user;
    } catch (const pqxx::sql_error& e) {
        common::Logger::getLogger().error("UserDAO findByUsername error: " + std::string(e.what()));
        throw;
    }
}

User UserDAO::findById(int userId) {
    // 逻辑同findByUsername，SQL改为WHERE id = $1，略
}