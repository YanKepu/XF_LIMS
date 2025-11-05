#include <jwt.h>
#include <chrono>
#include <Poco/Logger.h>
#include "common/JwtUtil.h"
#include "common/Config.h"
#include "common/Logger.h"

static std::string g_secret;
static int g_expire_seconds;

void JwtUtil::init() {
    auto& config = Config::getInstance();
    g_secret = config.getString("JWT.secret");
    g_expire_seconds = config.getInt("JWT.expire_seconds", 86400);
    if (g_secret.empty()) {
        throw std::runtime_error("JWT secret is empty");
    }
}

std::string JwtUtil::generate(const std::map<std::string, std::string>& claims) {
    jwt_t* jwt = jwt_new();
    if (!jwt) {
        throw std::runtime_error("Failed to create JWT");
    }

    // 设置算法（HS256）
    if (jwt_set_alg(jwt, JWT_ALG_HS256, (const unsigned char*)g_secret.c_str(), g_secret.size()) != 0) {
        jwt_free(jwt);
        throw std::runtime_error("Failed to set JWT algorithm");
    }

    // 设置过期时间
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    jwt_add_claim_int(jwt, "exp", now + g_expire_seconds);

    // 添加自定义claims
    for (const auto& [k, v] : claims) {
        jwt_add_claim_str(jwt, k.c_str(), v.c_str());
    }

    // 生成令牌
    char* token = jwt_encode_str(jwt);
    std::string result(token);
    jwt_free_str(token);
    jwt_free(jwt);
    return result;
}

bool JwtUtil::verify(const std::string& token, std::map<std::string, std::string>& claims) {
    jwt_t* jwt;
    int ret = jwt_decode(&jwt, token.c_str(), (const unsigned char*)g_secret.c_str(), g_secret.size());
    if (ret != 0) {
        Logger::get().error("JWT verify failed: %s", jwt_strerror(ret));
        return false;
    }

    // 解析claims
    const jwt_claim_t* claim = jwt_first_claim(jwt);
    while (claim) {
        claims[claim->name] = claim->val;
        claim = jwt_next_claim(claim);
    }

    jwt_free(jwt);
    return true;
}