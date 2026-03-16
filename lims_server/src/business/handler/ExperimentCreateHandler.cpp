#include "business/handler/ExperimentCreateHandler.h"
#include "common/Logger.h"
#include "service/ExperimentService.h" // Include ExperimentService for instance access
#include "common/ProcessResult.h" // Include ProcessResult

ExperimentCreateHandler::ExperimentCreateHandler() {
    // Constructor for CommandRouter registration (no direct dependency injection here)
}

ProcessResult ExperimentCreateHandler::handle(const nlohmann::json& reqData, nlohmann::json& response) {
    ProcessResult result;
    result.successflag = false;
    result.code = 500; // Default error code
    result.msg = "服务器内部错误。";

    // 1. Validate request and extract data
    if (!reqData.contains("experiment_name") || !reqData.contains("experiment_goal") ||
        !reqData.contains("start_date") || !reqData.contains("end_date") ||
        !reqData.contains("status")) {
        result.code = 400;
        result.msg = "缺少必填实验字段。";
        return result;
    }

    try {
        std::string name = reqData["experiment_name"];
        std::string goal = reqData["experiment_goal"];
        std::string startDate = reqData["start_date"];
        std::string endDate = reqData["end_date"];
        std::string description = reqData.contains("description") ? reqData["description"].get<std::string>() : "";
        std::string status = reqData["status"];

        // 2. Call ExperimentService to create experiment
        // Access ExperimentService singleton
        std::string experimentId = ExperimentService::instance().createExperiment(name, goal, startDate, endDate, description, status);

        if (!experimentId.empty()) {
            common::Logger::getLogger().information("ExperimentCreateHandler", "Experiment created successfully: " + name);
            result.successflag = true;
            result.code = 200;
            result.msg = "实验创建成功！";
            response["experiment_id"] = experimentId;
            response["experiment_name"] = name;
        } else {
            common::Logger::getLogger().error("ExperimentCreateHandler", "Failed to create experiment: " + name);
            result.msg = "实验创建失败。可能实验名称已存在或数据库错误。";
        }
    } catch (const nlohmann::json::exception& e) {
        common::Logger::getLogger().error("ExperimentCreateHandler", "JSON parsing error: " + std::string(e.what()));
        result.code = 400;
        result.msg = "JSON解析错误。";
    } catch (const std::exception& e) {
        common::Logger::getLogger().error("ExperimentCreateHandler", "Error creating experiment: " + std::string(e.what()));
        result.msg = "服务器内部错误。";
    }
    return result;
}
