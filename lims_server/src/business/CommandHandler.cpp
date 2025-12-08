// src/business/CommandRouter.cpp
#include "business/CommandRouter.h"
#include "common/Logger.h"

namespace business {

CommandRouter& CommandRouter::instance() {
    static CommandRouter router;        // 全局单例模式
    return router;
}

std::shared_ptr<abstract::ICommandHandler> CommandRouter::getHandler(const std::string& cmd) {
    auto it = _handlerMap.find(cmd);
    return (it != _handlerMap.end()) ? it->second : nullptr;
}

} // namespace business