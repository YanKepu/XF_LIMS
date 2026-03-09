#include "service/UserService.h"
#include "common/Logger.h"
#include "common/bcrypt.h"
#include <regex>

void UserService::validateUsername(const std::string& username) {
    // 用户名规则：6-20位，字母/数字/下划线
    std::regex regex("^[a-zA-Z0-9_]{6,20}$");
    if (!std::regex_match(username, regex)) {
        common::Logger::getLogger().warning("UserService validateUsername failed: " + username);
        throw std::runtime_error("username invalid (6-20 chars: letter/number/_)");
    }
}

void UserService::validatePassword(const std::string& password) {
    // 密码规则：8-20位，字母+数字
    std::regex regex("^(?=.*[a-zA-Z])(?=.*\\d).{8,20}$");
    if (!std::regex_match(password, regex)) {
        throw std::runtime_error("password invalid (8-20 chars: letter + number)");
    }
}

void UserService::registerUser(const std::string& username, const std::string& password, const std::string& phone) {
    // 1. 参数校验
    validateUsername(username);
    validatePassword(password);

    // 2. 检查用户名是否已存在
    if (userDAO->existsByUsername(username)) {
        throw std::runtime_error("username already exists");
    }

    // 3. bcrypt加密密码（复用你现有bcrypt代码）
    std::string encryptedPwd = bcrypt::generateHash(password); // 需确保你的bcrypt有generateHash方法

    // 4. 构建User对象
    User user;
    user.setUsername(username);
    user.setPassword(encryptedPwd);
    user.setPhone(phone);

    // 5. 调用DAO插入
    userDAO->insertUser(user);
    common::Logger::getLogger().information("UserService registerUser success: " + username);
}

std::string UserService::loginUser(const std::string& username, const std::string& password) {
    // 1. 查询用户
    User user = userDAO->findByUsername(username);

    // 2. 验证bcrypt密码
    if (!bcrypt::validatePassword(password, user.getPassword())) { // 复用bcrypt验证方法
        common::Logger::getLogger().warning("UserService loginUser password error: " + username);
        throw std::runtime_error("password error");
    }

    std::string token;
    // 3. 生成JWT Token（复用你的JwtUtil）
    // std::string token = JwtUtil::generateToken(user.getId()); // 需确保JwtUtil::generateToken已实现
    common::Logger::getLogger().information("UserService loginUser success: " + username);
    return token;
}

User UserService::getUserInfo(int userId) {
    User user = userDAO->findById(userId);
    user.setPassword(""); // 脱敏：清空密码
    return user;
}