// 示例：UserDAO.h
#pragma once
#include "DBConnection.h"
#include <memory>
class UserDAO {
public:
    // 构造传参（保留测试友好性）
    UserDAO(std::shared_ptr<DBConnection> dbConn) : _dbConn(dbConn) {}
    void insertUser(const std::string& username) {
        _dbConn->execute("INSERT INTO user VALUES ('" + username + "')");
    }
private:
    std::shared_ptr<DBConnection> _dbConn; // 用 shared_ptr 避免悬空引用
};

