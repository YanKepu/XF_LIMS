// 示例：DBConnection.h（核心依赖）
#pragma once
#include <string>
class DBConnection {
public:
    DBConnection(const std::string& dbUrl = "mysql://127.0.0.1:3306/lims") : _dbUrl(dbUrl) {
        // 初始化数据库连接
    }
    // 数据库操作方法
    void execute(const std::string& sql) {}

    void create();
private:
    std::string _dbUrl;
};

