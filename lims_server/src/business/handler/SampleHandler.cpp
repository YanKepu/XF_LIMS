#include "business/handler/SampleHandler.h"
#include "common/Logger.h"
#include "service/SampleService.h"

namespace business::handler {

SampleHandler::SampleHandler() {
    // Constructor for CommandRouter registration
}

std::string SampleHandler::getCmd() const {
    return "sample";
}

ProcessResult SampleHandler::handle(const json& reqData, json& response) {
    if (!reqData.contains("action")) {
        ProcessResult result;
        result.successflag = false;
        result.code = 400;
        result.msg = "缺少action字段";
        return result;
    }

    std::string action = reqData["action"];

    if (action == "register_sample") {
        return handleRegister(reqData, response);
    } else if (action == "get_sample_list") {
        return handleList(reqData, response);
    } else {
        ProcessResult result;
        result.successflag = false;
        result.code = 400;
        result.msg = "未知的action: " + action;
        return result;
    }
}

ProcessResult SampleHandler::handleRegister(const json& reqData, json& response) {
    ProcessResult result;
    result.successflag = false;
    result.code = 500;
    result.msg = "服务器内部错误。";

    if (!reqData.contains("code") || !reqData.contains("name") || !reqData.contains("customer")) {
        result.code = 400;
        result.msg = "缺少必填样品字段。";
        return result;
    }

    try {
        SampleService service;
        nlohmann::json serviceResponse = service.registerSample(reqData);
        
        if (serviceResponse["code"] == 200) {
            common::Logger::getLogger().information("SampleHandler", "样品登记成功: " + reqData["name"].get<std::string>());
            result.successflag = true;
            result.code = 200;
            result.msg = "样品登记成功！";
            response["sample_id"] = serviceResponse["data"]["sample_id"];
            response["sample_name"] = reqData["name"].get<std::string>();
        } else {
            common::Logger::getLogger().error("SampleHandler", "样品登记失败: " + serviceResponse["msg"].get<std::string>());
            result.msg = serviceResponse["msg"].get<std::string>();
        }
    } catch (const nlohmann::json::exception& e) {
        common::Logger::getLogger().error("SampleHandler", "JSON parsing error: " + std::string(e.what()));
        result.code = 400;
        result.msg = "JSON解析错误。";
    } catch (const std::exception& e) {
        common::Logger::getLogger().error("SampleHandler", "Error registering sample: " + std::string(e.what()));
        result.msg = "服务器内部错误。";
    }
    return result;
}

ProcessResult SampleHandler::handleList(const json& reqData, json& response) {
    ProcessResult result;
    result.successflag = false;
    result.code = 500;
    result.msg = "服务器内部错误。";

    try {
        SampleService service;
        nlohmann::json serviceResponse = service.getSampleList();
        
        if (serviceResponse["code"] == 200) {
            common::Logger::getLogger().information("SampleHandler", "获取样品列表成功");
            result.successflag = true;
            result.code = 200;
            result.msg = "获取样品列表成功";
            response["samples"] = serviceResponse["data"]["samples"];
        } else {
            common::Logger::getLogger().error("SampleHandler", "获取样品列表失败: " + serviceResponse["msg"].get<std::string>());
            result.msg = serviceResponse["msg"].get<std::string>();
        }
    } catch (const nlohmann::json::exception& e) {
        common::Logger::getLogger().error("SampleHandler", "JSON parsing error: " + std::string(e.what()));
        result.code = 400;
        result.msg = "JSON解析错误。";
    } catch (const std::exception& e) {
        common::Logger::getLogger().error("SampleHandler", "Error getting sample list: " + std::string(e.what()));
        result.msg = "服务器内部错误。";
    }
    return result;
}

} // namespace business::handler
