#include <jwt.h>
#include <chrono>
#include <string>
#include <map>
#include <stdexcept>
#include "common/Logger.h"
#include "common/Config.h"
#include "common/JwtUtil.h"

static std::string g_secret;
static int g_expire_seconds;

void JwtUtil::init() {
    auto& config = common::Config::getInstance();
    g_secret = config.getString("JWT.secret");
    g_expire_seconds = config.getInt("JWT.expire_seconds", 86400); // 默认24小时
    if (g_secret.empty()) {
        throw std::runtime_error("JWT secret is empty");
    }
}

std::string JwtUtil::generate(const std::map<std::string, std::string>& claims) {
    jwt_t* jwt = nullptr;

    // 1. 创建 JWT 对象
    if (jwt_new(&jwt) != 0 || !jwt) {
        throw std::runtime_error("Failed to create JWT (jwt_new)");
    }

    // 2. 设置签名算法 (HS256)
    if (jwt_set_alg(jwt, JWT_ALG_HS256,
                   reinterpret_cast<const unsigned char*>(g_secret.c_str()),
                   g_secret.size()) != 0) {
        jwt_free(jwt);
        throw std::runtime_error("Failed to set JWT algorithm (jwt_set_alg)");
    }

    // 3. 设置过期时间 (exp grant)
    //    使用 jwt_add_grant_int，因为你的库用 "grant" 命名
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    if (jwt_add_grant_int(jwt, "exp", now + g_expire_seconds) != 0) {
        jwt_free(jwt);
        throw std::runtime_error("Failed to add 'exp' grant (jwt_add_grant_int)");
    }

    // 4. 添加自定义 claims (使用 jwt_add_grant_str)
    for (std::map<std::string, std::string>::const_iterator it = claims.begin(); it != claims.end(); ++it) {
    const std::string& key = it->first;
    const std::string& value = it->second;
        if (jwt_add_grant(jwt, key.c_str(), value.c_str()) != 0) {
            jwt_free(jwt);
            throw std::runtime_error("Failed to add grant: " + key);
        }
    }

    // 5. 生成 JWT 字符串
    char* token_str = jwt_encode_str(jwt);
    if (!token_str) {
        jwt_free(jwt);
        throw std::runtime_error("Failed to encode JWT (jwt_encode_str)");
    }

    // 6. 释放资源
    std::string result(token_str);
    jwt_free(jwt);

    return result;
}

bool JwtUtil::verify(const std::string& token, std::map<std::string, std::string>& claims) {
    jwt_t* jwt = nullptr;

    return true;
}