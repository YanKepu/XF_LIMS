#pragma once
#include <string>
#include <map>

class JwtUtil {
public:
    static void init();  // 从配置加载密钥
    static std::string generate(const std::map<std::string, std::string>& claims);  // 生成令牌
    static bool verify(const std::string& token, std::map<std::string, std::string>& claims);  // 验证令牌并解析claims
};