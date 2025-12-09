// src/business/handler/UserLoginHandler.cpp
#include <iostream>
#include <pqxx/pqxx>
#include <stdexcept>
#include "business/handler/UserLoginHandler.h"
#include "common/Logger.h"
#include "nlohmann/json.hpp"
#include "dao/DBConnection.h"
#include "common/Config.h"
#include "common/cryptoUtils.h"



using json = nlohmann::json;

// 绑定命令："user_login"
std::string business::handler::UserLoginHandler::getCmd() const {
    return "user_login";
}

// 具体业务逻辑
ProcessResult business::handler::UserLoginHandler::handle(const json& reqData, json& response) {
    ProcessResult result;
    std::shared_ptr<pqxx::connection> dbConn = nullptr;

    try {
        // 从业务数据中提取字段（reqData 是 reqJson["data"]）
        std::string username = reqData.value("username", "");
        std::string password = reqData.value("password", "");

        std::cout << "进入userlogin流程！！" << std::endl;

        dbConn = DBConnection::getConnection();
        if(!dbConn || !dbConn->is_open()) {
            throw std::runtime_error("无法获取数据库连接");
        }

        /* step 3 开启事务 */
        pqxx::work txn(*dbConn);    /* Postgre事务 */
        std::string sql = R"(
            SELECT username, password
            FROM users 
            WHERE username = $1)
        )";   // $1 是参数占位符
        pqxx::result res = txn.exec_params(sql, username);
        txn.commit();           /* 只读查询也commit，避免事务残留 */

        /* step 4 校验用户是否存在 */
        if(res.empty()){
            result.successflag = false;
            result.code = 401;
            result.msg = "用户名或密码错误";
            common::Logger::getLogger().error("用户登录失败（用户不存在）：%s", username.c_str());
            return result;
        }

        /* step 5 提取用户数据 */
        std::string dbPassword = res[0]["password"].c_str();    /* 获取数据库存储的密码哈希值(含盐值) */
        bool is_enable = res[0]["is_enable"].as<bool>();        /*是否启用*/

        /* step 6 验证用户可用性 */
        if(!is_enable){
            result.successflag = false;
            result.code = 403;
            result.msg = "用户已被禁用";
            common::Logger::getLogger().error("用户登录失败（用户禁用）：%s", username.c_str());
            return result;
        }   

        /* step 7 验证密码 */
        if (bcrypt::validatePassword(password, dbPassword)) {
            result.successflag = true;
            result.code = 200;
            result.msg = "登录成功";
            response["data"] = {{"token", "abcdef123456"}};     // 生产环境使用JWT
            common::Logger::getLogger().information("用户登录成功：%s", username.c_str());
        } else {
            result.successflag = false;
            result.code = 401;
            result.msg = "用户名或密码错误";
            common::Logger::getLogger().error("用户登录失败：%s", username.c_str());
        }
    } catch (const pqxx::sql_error& e) {
        result.successflag = false;
        result.code = 500;
        result.msg = "数据库查询错误：" + std::string(e.what());
        common::Logger::getLogger().error("UserLoginHandler SQL 错误：%s，查询：%s", e.what(), e.query().c_str());
    } catch (const std::exception& e) {
        result.successflag = false;
        result.code = 500;
        result.msg = "登录处理异常：" + std::string(e.what());
        common::Logger::getLogger().error("UserLoginHandler 异常：%s", e.what());
    }
    return result;
}

