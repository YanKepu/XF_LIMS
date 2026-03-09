#pragma once
#include "dao/UserDAO.h"
#include "model/User.h"
#include "common/JwtUtil.h"
#include "common/bcrypt.h" // 复用现有bcrypt
#include <string>

// 用户业务层：封装登录/注册逻辑，不直接操作TCP/数据库
class UserService {
private:
    std::shared_ptr<UserDAO> userDAO; // 依赖注入UserDAO（避免硬编码）

    // 私有：参数校验
    void validateUsername(const std::string& username);
    void validatePassword(const std::string& password);

public:
    UserService(std::shared_ptr<UserDAO> dao) : userDAO(dao) {}

    // 注册业务
    void registerUser(const std::string& username, const std::string& password, const std::string& phone);

    // 登录业务（返回JWT Token）
    std::string loginUser(const std::string& username, const std::string& password);

    // 查询用户信息（脱敏）
    User getUserInfo(int userId);
};