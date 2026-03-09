// 示例：UserService.h
#pragma once
#include "UserDAO.h"
#include <memory>
class UserService {
public:
    // 构造传参（保留测试友好性）
    UserService(std::shared_ptr<UserDAO> userDAO) : _userDAO(userDAO) {}
    void registerUser(const std::string& username) {
        _userDAO->insertUser(username);
    }
private:
    std::shared_ptr<UserDAO> _userDAO;
};