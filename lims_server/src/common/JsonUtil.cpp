

#include "common/JsonUtil.h"
#include "common/Logger.h"


/* -------------------------- 解析 JSON 字符串（反序列化）-------------------------- */
json JsonUtil::parse(const std::string& jsonStr) {
    try {
        // nlohmann::json::parse() 自动校验 JSON 格式，失败会抛异常
        return json::parse(jsonStr);
    } catch (const nlohmann::json::parse_error& e) {
        // 捕获解析异常，封装成自定义异常（附带错误位置）
        std::string errMsg = 
            "JSON解析失败：" + std::string(e.what()) + 
            "（错误位置：第" + std::to_string(e.byte) + "字节）";
        common::Logger::getLogger().error(errMsg);
        throw JsonParseException(errMsg);  // 抛给业务层处理
    }
}

/***********************  生成Json字符串 （序列化）**************************** */
std::string JsonUtil::stringify(const json& jsonObj, bool pretty) {
    if (pretty) {
        return jsonObj.dump(4);  // 格式化输出，缩进4个空格
    } else {
        return jsonObj.dump();    // 紧凑输出
    }
}
/***********************  校验必填字段 **************************** */
void JsonUtil::checkRequiredFields(const json& jsonObj, const std::vector<std::string>& requiredFields) {
    for (const auto& field : requiredFields) {
        if ((!jsonObj.contains(field)) || jsonObj[field].is_null()) {
            common::Logger::getLogger().error("缺失必填字段：%s", field);
            throw JsonFieldMissingException(field);  // 抛给业务层处理
        }
    }
}

/**********************  创建统一响应格式 *************************************/
json JsonUtil::createResponse(int code, const std::string& msg, const json& data) {
    json response;
    response["code"] = code;
    response["msg"] = msg;
    response["data"] = data;
    return response;
}