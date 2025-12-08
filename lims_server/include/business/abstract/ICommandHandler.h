#ifndef __ICOMMANDHANDLER__
#define __ICOMMANDHANDLER__
#include "nlohmann/json_fwd.hpp"
#include "common/ProcessResult.h"

using json = nlohmann::json;


//程序启动 → main函数 → 初始化模块 → 调用 CommandRouter::registerHandler(处理器实例) → 注册完成 → 后续请求可正常路由

namespace business {
    namespace abstract {
        // 🔥 抽象层核心接口：所有业务模块必须实现
        class ICommandHandler {
        public:
            virtual ~ICommandHandler() = default;  // 虚析构：确保子类正确析构
            // 核心处理方法：入参=业务数据（reqJson["data"]），出参=响应JSON，返回=处理结果
            virtual ProcessResult handle(const json& reqData, json& response) = 0;
            // 获取绑定的命令（如 "user_login"）
            virtual std::string getCmd() const = 0;
        };

    } // namespace abstract
} // namespace business
#endif