#include "business/handler/UserRegisterHandler.h"
#include "common/JsonUtil.h"
#include "common/Logger.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;


// 绑定命令："user_login"
std::string business::handler::UserRegisterHandler::getCmd() const {
    return "user_register";
}

ProcessResult business::handler::UserRegisterHandler::handle(const json& reqData, json& response) {
    ProcessResult result;
    json resp;
    try {
        // 解析TCP请求JSON
        // 从业务数据中提取字段（reqData 是 reqJson["data"]）
        std::string username = reqData.value("username", "");
        std::string password = reqData.value("password", "");
        std::string phone = reqData.value("phone", "");

        // 调用Service注册
        userService->registerUser(username, password, phone);

        // 成功响应
        result.code = 0;
        result.msg = "register success";
    } catch (const std::exception& e) {
        result.code = 1;
        result.msg = e.what();
        common::Logger::getLogger().error("UserRegisterHandler handle error: " + std::string(e.what()));
    }


    return result;
}