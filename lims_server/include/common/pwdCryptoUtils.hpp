#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include <vector>
#include <argon2.h>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <pqxx/pqxx>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

// -------------------------- 配置项 --------------------------
const std::string AES_KEY = "0123456789ABCDEF0123456789ABCDEF";  // 32字节
// Argon2id参数（与Python完全对齐）
const uint32_t ARGON2_TIME_COST = 3;
const uint32_t ARGON2_MEMORY_COST = 65536;
const uint32_t ARGON2_PARALLELISM = 4;
const uint32_t ARGON2_HASH_LEN = 32;
const uint32_t ARGON2_SALT_LEN = 16;
// TCP配置
const int SERVER_PORT = 8888;
// 数据库配置
const std::string DB_CONN_STR = "dbname=lims_db user=lims_user password=your_password host=localhost";

// -------------------------- 工具函数 --------------------------
// 字符串分割
std::vector<std::string> split(const std::string& s, char delim) {
    std::vector<std::string> res;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        res.push_back(item);
    }
    return res;
}

// 16进制字符串转字节
bool hex2bytes(const std::string& hex_str, std::vector<unsigned char>& bytes) {
    bytes.clear();
    if (hex_str.length() % 2 != 0) return false;
    for (size_t i = 0; i < hex_str.length(); i += 2) {
        std::string byte_str = hex_str.substr(i, 2);
        char* end;
        unsigned char byte = strtol(byte_str.c_str(), &end, 16);
        if (end == byte_str.c_str()) return false;
        bytes.push_back(byte);
    }
    return true;
}

// -------------------------- AES-GCM解密 --------------------------
std::string aes_gcm_decrypt(const std::string& cipher_hex, 
                           const std::string& iv_hex, 
                           const std::string& tag_hex) {
    // 1. 16进制转字节
    std::vector<unsigned char> cipher_bytes, iv_bytes, tag_bytes;
    if (!hex2bytes(cipher_hex, cipher_bytes) || 
        !hex2bytes(iv_hex, iv_bytes) || 
        !hex2bytes(tag_hex, tag_bytes)) {
        throw std::runtime_error("16进制解析失败");
    }
    // 2. AES-GCM解密
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) throw std::runtime_error("EVP_CTX创建失败");

    int ret = EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, 
                                (const unsigned char*)AES_KEY.c_str(), iv_bytes.data());
    if (ret != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("DecryptInit失败");
    }
    // 设置认证标签
    ret = EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, tag_bytes.size(), tag_bytes.data());
    if (ret != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("设置Tag失败");
    }
    // 解密
    std::vector<unsigned char> plain_bytes(cipher_bytes.size() + AES_BLOCK_SIZE);
    int plain_len = 0;
    ret = EVP_DecryptUpdate(ctx, plain_bytes.data(), &plain_len, 
                           cipher_bytes.data(), cipher_bytes.size());
    if (ret != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("DecryptUpdate失败");
    }
    // 结束解密（验证标签）
    int final_len = 0;
    ret = EVP_DecryptFinal_ex(ctx, plain_bytes.data() + plain_len, &final_len);
    EVP_CIPHER_CTX_free(ctx);
    if (ret != 1) {
        throw std::runtime_error("Tag验证失败（数据被篡改）");
    }
    // 去填充
    plain_len += final_len;
    std::string plain_text((char*)plain_bytes.data(), plain_len);
    return plain_text;
}

// -------------------------- Argon2id验证 --------------------------
bool argon2id_verify(const std::string& hash_str, const std::string& plain_pwd) {
    // Argon2id验证（直接用argon2库的验证函数，自动解析哈希串中的参数/盐值）
    int ret = argon2id_verify(
        hash_str.c_str(),
        plain_pwd.c_str(),
        plain_pwd.length()
    );
    return ret == ARGON2_OK;
}

// -------------------------- 数据库操作 --------------------------
// 注册用户：存储Argon2id哈希
bool db_register_user(const std::string& username, const std::string& argon2_hash) {
    try {
        pqxx::connection db_conn(DB_CONN_STR);
        if (!db_conn.is_open()) {
            std::cerr << "数据库连接失败" << std::endl;
            return false;
        }
        pqxx::work txn(db_conn);
        // 创建users表（若不存在，仅首次执行）
        txn.exec("CREATE TABLE IF NOT EXISTS users ("
                 "id SERIAL PRIMARY KEY, "
                 "username VARCHAR(50) UNIQUE NOT NULL, "
                 "argon2_hash VARCHAR(512) NOT NULL)");
        // 插入用户
        std::string sql = "INSERT INTO users (username, argon2_hash) VALUES ($1, $2)";
        txn.exec_params(sql, username, argon2_hash);
        txn.commit();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "注册数据库错误：" << e.what() << std::endl;
        return false;
    }
}

// 查询用户的Argon2id哈希
std::string db_get_user_hash(const std::string& username) {
    try {
        pqxx::connection db_conn(DB_CONN_STR);
        if (!db_conn.is_open()) {
            return "";
        }
        pqxx::read_only_work txn(db_conn);
        std::string sql = "SELECT argon2_hash FROM users WHERE username = $1";
        pqxx::result res = txn.exec_params(sql, username);
        txn.commit();
        if (res.empty()) return "";
        return res[0]["argon2_hash"].as<std::string>();
    } catch (const std::exception& e) {
        std::cerr << "查询哈希错误：" << e.what() << std::endl;
        return "";
    }
}

// -------------------------- TCP服务端处理 --------------------------
void handle_client(int conn_sock) {
    char buf[2048] = {0};
    int recv_len = recv(conn_sock, buf, sizeof(buf)-1, 0);
    if (recv_len <= 0) {
        close(conn_sock);
        return;
    }
    std::string recv_data(buf, recv_len);
    std::cout << "收到客户端数据：" << recv_data << std::endl;

    try {
        // 1. 解析传输数据：cipher_hex|iv_hex|tag_hex
        std::vector<std::string> parts = split(recv_data, '|');
        if (parts.size() != 3) {
            send(conn_sock, "数据格式错误", strlen("数据格式错误"), 0);
            close(conn_sock);
            return;
        }
        std::string cipher_hex = parts[0];
        std::string iv_hex = parts[1];
        std::string tag_hex = parts[2];

        // 2. AES解密
        std::string plain_data = aes_gcm_decrypt(cipher_hex, iv_hex, tag_hex);
        std::cout << "解密后数据：" << plain_data << std::endl;

        // 3. 解析业务数据：REGISTER|username|hash 或 LOGIN|username|pwd
        std::vector<std::string> biz_parts = split(plain_data, '|');
        if (biz_parts.size() != 3) {
            send(conn_sock, "业务数据格式错误", strlen("业务数据格式错误"), 0);
            close(conn_sock);
            return;
        }
        std::string cmd = biz_parts[0];
        std::string username = biz_parts[1];
        std::string content = biz_parts[2];

        // 4. 处理注册/登录
        if (cmd == "REGISTER") {
            // 注册：存储Argon2id哈希
            bool success = db_register_user(username, content);
            if (success) {
                send(conn_sock, "注册成功", strlen("注册成功"), 0);
            } else {
                send(conn_sock, "注册失败", strlen("注册失败"), 0);
            }
        } else if (cmd == "LOGIN") {
            // 登录：查询哈希并验证
            std::string stored_hash = db_get_user_hash(username);
            if (stored_hash.empty()) {
                send(conn_sock, "用户不存在", strlen("用户不存在"), 0);
            } else {
                bool verify_ok = argon2id_verify(stored_hash, content);
                if (verify_ok) {
                    send(conn_sock, "登录成功", strlen("登录成功"), 0);
                } else {
                    send(conn_sock, "密码错误", strlen("密码错误"), 0);
                }
            }
        } else {
            send(conn_sock, "未知命令", strlen("未知命令"), 0);
        }
    } catch (const std::exception& e) {
        std::cerr << "处理错误：" << e.what() << std::endl;
        send(conn_sock, ("处理失败：" + std::string(e.what())).c_str(), strlen(e.what()) + 7, 0);
    }
    close(conn_sock);
}

