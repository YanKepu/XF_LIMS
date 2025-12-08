#ifndef JSONUTIL_H
#define JSONUTIL_H
#include <nlohmann/json.hpp>
#include <string>
#include <vector>
#include <stdexcept>

using json = nlohmann::json;

/***************  自定义Json异常（用于业务层捕捉）  ************** */
class JsonParseException : public std::runtime_error {
public:
    explicit JsonParseException(const std::string& message)
        : std::runtime_error("JSON Parse Error: " + message) {}
};

class JsonFieldMissingException : public std::runtime_error {
public:
    explicit JsonFieldMissingException(const std::string& fieldName)
        : std::runtime_error("JSON Field Missing: " + fieldName) {}
};

/************  Json工具类（统一处理序列化/反序列化/校验）  ******** */

class JsonUtil {
public:
/**
     * 反序列化：JSON 字符串 → Json（带格式校验）
     * @param jsonStr 输入的 JSON 字符串
     * @return 解析后的 JSON 对象
     * @throw JsonParseException JSON 格式错误
     */
    static json parse(const std::string& jsonStr);

    /**
     * 序列化：jsont → JSON 字符串（支持格式化/压缩）
     * @param jsonObj 要打包的 JSON 对象
     * @param pretty 是否格式化输出（调试用 true，生产用 false 节省带宽）
     * @return 打包后的 JSON 字符串
     */
    static std::string stringify(const json& jsonObj, bool pretty = false);

    /**
     * 校验 JSON 对象是否包含必填字段
     * @param jsonObj 待校验的 JSON 对象
     * @param requiredFields 必填字段列表
     * @throw JsonFieldMissingException 缺失字段
     */
    static void checkRequiredFields(const json& jsonObj, const std::vector<std::string>& requiredFields);

    /**
     * 快速创建业务响应 JSON（统一响应格式）
     * @param code 状态码（0=成功，非0=失败）
     * @param msg 提示信息
     * @param data 业务数据（可选）
     * @return 响应 JSON 对象
     */
    static json createResponse(int code, const std::string& msg, const json& data = json{});
};



#endif // JSONUTIL_H