// src/business/handler/UserLoginHandler.cpp
#include "business/handler/UserLoginHandler.h"
#include "common/Logger.h"
#include "nlohmann/json.hpp"
#include <iostream>

using json = nlohmann::json;

// 绑定命令："user_login"
std::string business::handler::UserLoginHandler::getCmd() const {
    return "user_login";
}

// 具体业务逻辑
ProcessResult business::handler::UserLoginHandler::handle(const json& reqData, json& response) {
    ProcessResult result;
    try {
        // 从业务数据中提取字段（reqData 是 reqJson["data"]）
        std::string username = reqData.value("username", "");
        std::string password = reqData.value("password", "");

        std::cout << "进入userlogin流程！！" << std::endl;
        // 调用登录核心逻辑（可进一步抽离到 service 层）
        if (username == "admin" && password == "123456") {
            result.successflag = true;
            result.code = 200;
            result.msg = "登录成功";
            response["data"] = {{"token", "abcdef123456"}};
            common::Logger::getLogger().information("用户登录成功：%s", username.c_str());
        } else {
            result.successflag = false;
            result.code = 401;
            result.msg = "用户名或密码错误";
            common::Logger::getLogger().error("用户登录失败：%s", username.c_str());
        }
    } catch (const std::exception& e) {
        result.successflag = false;
        result.code = 500;
        result.msg = "登录处理异常：" + std::string(e.what());
        common::Logger::getLogger().error("UserLoginHandler 异常：%s", e.what());
    }
    return result;
}

