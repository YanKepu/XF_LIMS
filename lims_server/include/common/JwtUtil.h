#pragma once
#include <string>
#include <map>

class JwtUtil {
public:
    JwtUtil(std::string secret, int expireSeconds)
        : secret_(secret), expireSeconds_(expireSeconds) {
        if(secret_.size() < 32){
            throw std::runtime_error("JWT secret must be at least 32 characters long");
        }
    }

    static void init();  // 从配置加载密钥
    static std::string generate(const std::map<std::string, std::string>& claims);  // 生成令牌
    static bool verify(const std::string& token, std::map<std::string, std::string>& claims);  // 验证令牌并解析claims
private:
    std::string secret_;
    int expireSeconds_;
};