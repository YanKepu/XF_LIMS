#pragma once


#include <string>
#include <pqxx/pqxx>
#include "DBConnection.h"
#include "model/User.h"

// 用户DAO：仅做数据库CRUD，无业务逻辑
class UserDAO {
private:
    std::shared_ptr<DBConnection> dbConn; // 复用现有数据库连接（避免重复创建）

public:
    // 构造：注入现有DBConnection（你的DBConnection应该是单例/全局）
    UserDAO(std::shared_ptr<DBConnection> conn) : dbConn(conn) {}
    // 检查用户名是否存在
    bool existsByUsername(const std::string& username);

    // 新增用户（注册）
    void insertUser(const User& user);

    // 根据用户名查询用户（登录）
    User findByUsername(const std::string& username);

    // 根据用户ID查询（后续查信息用）
    User findById(int userId);
};