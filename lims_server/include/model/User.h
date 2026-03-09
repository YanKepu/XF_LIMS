#pragma once
#include <string>
#include <ctime>

// 对应数据库user表
class User {
private:
    int id;                 // 主键ID
    std::string username;   // 用户名（唯一）
    std::string password;   // bcrypt加密后的密码
    std::string phone;      // 手机号（TCP场景可简化，无邮箱也可）
    int status;             // 1-正常，0-禁用
    std::time_t create_time;// 创建时间

public:
    User() : id(0), status(1), create_time(time(nullptr)) {}

    // Getter/Setter（必须实现，供DAO/Service调用）
    int getId() const { return id; }
    void setId(int id) { this->id = id; }

    std::string getUsername() const { return username; }
    void setUsername(const std::string& username) { this->username = username; }

    std::string getPassword() const { return password; }
    void setPassword(const std::string& password) { this->password = password; }

    std::string getPhone() const { return phone; }
    void setPhone(const std::string& phone) { this->phone = phone; }

    int getStatus() const { return status; }
    void setStatus(int status) { this->status = status; }

    time_t getCreateTime() const { return create_time; }
    void setCreateTime(time_t t) { this->create_time = t; }
};