#pragma once
#include <string>
#include "common/bcrypt.h"  // 假设使用一个名为 BCrypt 的库

namespace business {
namespace utils {


// 验证密码（明文密码 + 盐值 + 数据库哈希 → 是否匹配）
inline bool verifyPassword(const std::string& password, const std::string& salt, const std::string& hash) {
    return bcrypt::validatePassword(password, hash);
}

} // namespace utils
} // namespace business