#pragma once
#include <pqxx/pqxx>
#include <memory>
#include <string>

// 数据库连接封装（基于libpqxx）
class DBConnection {
public:
    static void init();  // 初始化连接参数（从配置）
    static std::unique_ptr<pqxx::connection> create();  // 创建新连接
    static std::shared_ptr<pqxx::connection> getConnection();  // 获取连接实例
};