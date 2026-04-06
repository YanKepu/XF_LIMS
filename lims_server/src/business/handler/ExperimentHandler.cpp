#include "business/handler/ExperimentHandler.h"
#include "common/Logger.h"
#include "service/ExperimentService.h"

namespace business::handler {

ExperimentHandler::ExperimentHandler() {
    // Constructor for CommandRouter registration
}

std::string ExperimentHandler::getCmd() const {
    return "experiment";
}

ProcessResult ExperimentHandler::handle(const json& reqData, json& response) {
    if (!reqData.contains("action")) {
        ProcessResult result;
        result.successflag = false;
        result.code = 400;
        result.msg = "缺少action字段";
        return result;
    }

    std::string action = reqData["action"];

    if (action == "create_experiment") {
        return handleCreate(reqData, response);
    } else if (action == "get_experiment_list") {
        return handleList(reqData, response);
    } else if (action == "update_experiment") {
        return handleUpdate(reqData, response);
    } else if (action == "delete_experiment") {
        return handleDelete(reqData, response);
    } else {
        ProcessResult result;
        result.successflag = false;
        result.code = 400;
        result.msg = "未知的action: " + action;
        return result;
    }
}

ProcessResult ExperimentHandler::handleCreate(const json& reqData, json& response) {
    ProcessResult result;
    result.successflag = false;
    result.code = 500;
    result.msg = "服务器内部错误。";

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

        auto& service = ExperimentService::instance();
        std::string experimentId = service.createExperiment(name, goal, startDate, endDate, description, status);

        if (!experimentId.empty()) {
            common::Logger::getLogger().information("ExperimentHandler", "Experiment created successfully: " + name);
            result.successflag = true;
            result.code = 200;
            result.msg = "实验创建成功！";
            response["experiment_id"] = experimentId;
            response["experiment_name"] = name;
        } else {
            common::Logger::getLogger().error("ExperimentHandler", "Failed to create experiment: " + name);
            result.msg = "实验创建失败。可能实验名称已存在或数据库错误。";
        }
    } catch (const nlohmann::json::exception& e) {
        common::Logger::getLogger().error("ExperimentHandler", "JSON parsing error: " + std::string(e.what()));
        result.code = 400;
        result.msg = "JSON解析错误。";
    } catch (const std::exception& e) {
        common::Logger::getLogger().error("ExperimentHandler", "Error creating experiment: " + std::string(e.what()));
        result.msg = "服务器内部错误。";
    }
    return result;
}

ProcessResult ExperimentHandler::handleList(const json& reqData, json& response) {
    ProcessResult result;
    result.successflag = false;
    result.code = 500;
    result.msg = "服务器内部错误。";

    try {
        std::string searchTerm = reqData.contains("search_term") ? reqData["search_term"].get<std::string>() : "";
        int page = reqData.contains("page") ? reqData["page"].get<int>() : 1;
        int pageSize = reqData.contains("page_size") ? reqData["page_size"].get<int>() : 10;

        auto& service = ExperimentService::instance();
        auto experiments = service.getExperimentList(searchTerm, page, pageSize);
        int total = service.getExperimentCount(searchTerm);

        response["total"] = total;
        response["experiments"] = experiments;

        common::Logger::getLogger().information("ExperimentHandler", "Retrieved experiment list, total: " + std::to_string(total));
        result.successflag = true;
        result.code = 200;
        result.msg = "获取实验列表成功";
    } catch (const nlohmann::json::exception& e) {
        common::Logger::getLogger().error("ExperimentHandler", "JSON parsing error: " + std::string(e.what()));
        result.code = 400;
        result.msg = "JSON解析错误。";
    } catch (const std::exception& e) {
        common::Logger::getLogger().error("ExperimentHandler", "Error getting experiment list: " + std::string(e.what()));
        result.msg = "服务器内部错误。";
    }
    return result;
}

ProcessResult ExperimentHandler::handleUpdate(const json& reqData, json& response) {
    ProcessResult result;
    result.successflag = false;
    result.code = 500;
    result.msg = "服务器内部错误。";

    if (!reqData.contains("id")) {
        result.code = 400;
        result.msg = "缺少实验ID。";
        return result;
    }

    try {
        std::string id = reqData["id"];
        std::string name = reqData.contains("experiment_name") ? reqData["experiment_name"].get<std::string>() : "";
        std::string goal = reqData.contains("experiment_goal") ? reqData["experiment_goal"].get<std::string>() : "";
        std::string startDate = reqData.contains("start_date") ? reqData["start_date"].get<std::string>() : "";
        std::string endDate = reqData.contains("end_date") ? reqData["end_date"].get<std::string>() : "";
        std::string description = reqData.contains("description") ? reqData["description"].get<std::string>() : "";
        std::string status = reqData.contains("status") ? reqData["status"].get<std::string>() : "";

        auto& service = ExperimentService::instance();
        bool success = service.updateExperiment(id, name, goal, startDate, endDate, description, status);

        if (success) {
            common::Logger::getLogger().information("ExperimentHandler", "Experiment updated successfully: " + id);
            result.successflag = true;
            result.code = 200;
            result.msg = "实验更新成功！";
        } else {
            common::Logger::getLogger().error("ExperimentHandler", "Failed to update experiment: " + id);
            result.msg = "实验更新失败。可能实验不存在或数据库错误。";
        }
    } catch (const nlohmann::json::exception& e) {
        common::Logger::getLogger().error("ExperimentHandler", "JSON parsing error: " + std::string(e.what()));
        result.code = 400;
        result.msg = "JSON解析错误。";
    } catch (const std::exception& e) {
        common::Logger::getLogger().error("ExperimentHandler", "Error updating experiment: " + std::string(e.what()));
        result.msg = "服务器内部错误。";
    }
    return result;
}

ProcessResult ExperimentHandler::handleDelete(const json& reqData, json& response) {
    ProcessResult result;
    result.successflag = false;
    result.code = 500;
    result.msg = "服务器内部错误。";

    if (!reqData.contains("ids")) {
        result.code = 400;
        result.msg = "缺少实验ID列表。";
        return result;
    }

    try {
        auto& service = ExperimentService::instance();
        auto ids = reqData["ids"].get<std::vector<std::string>>();
        int deletedCount = 0;

        for (const auto& id : ids) {
            if (service.deleteExperiment(id)) {
                deletedCount++;
            }
        }

        common::Logger::getLogger().information("ExperimentHandler", "Deleted " + std::to_string(deletedCount) + " experiments");
        result.successflag = true;
        result.code = 200;
        result.msg = "实验删除成功！";
    } catch (const nlohmann::json::exception& e) {
        common::Logger::getLogger().error("ExperimentHandler", "JSON parsing error: " + std::string(e.what()));
        result.code = 400;
        result.msg = "JSON解析错误。";
    } catch (const std::exception& e) {
        common::Logger::getLogger().error("ExperimentHandler", "Error deleting experiments: " + std::string(e.what()));
        result.msg = "服务器内部错误。";
    }
    return result;
}

} // namespace business::handler