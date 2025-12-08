// include/business/CommandRouter.h
#pragma once

#include <memory>
#include <unordered_map>
#include <string>
#include "business/abstract/ICommandHandler.h"  // 依赖抽象接口
#include "common/Logger.h"

namespace business {

class CommandRouter {
public:
    static CommandRouter& instance();  // 单例入口

    // 🔥 主动注册接口：外部显式调用，传入处理器实例
    template <typename HandlerType>
    bool registerHandler() {
        // 创建处理器实例（要求 HandlerType 是 ICommandHandler 的子类）
        std::shared_ptr<abstract::ICommandHandler> handler = std::make_shared<HandlerType>();
        if (!handler) {
            common::Logger::getLogger().error("注册处理器失败：创建实例失败");
            return false;
        }
        std::string cmd = handler->getCmd();
        if (_handlerMap.count(cmd) > 0) {
            common::Logger::getLogger().error("注册处理器失败：cmd已重复：%s", cmd.c_str());
            return false;
        }
        _handlerMap.emplace(cmd, handler);
        common::Logger::getLogger().information("处理器注册成功：%s", cmd.c_str());
        return true;
    }

    // 根据 cmd 获取处理器（MessageHandler 调用）
    std::shared_ptr<abstract::ICommandHandler> getHandler(const std::string& cmd);

private:
    CommandRouter() = default;
    ~CommandRouter() = default;
    CommandRouter(const CommandRouter&) = delete;
    CommandRouter& operator=(const CommandRouter&) = delete;

    std::unordered_map<std::string, std::shared_ptr<abstract::ICommandHandler>> _handlerMap;
};

} // namespace business