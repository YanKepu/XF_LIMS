// main.cpp
#include "DIContainer.h"
#include "DBConnection.h"
#include "UserDAO.h"
#include "UserService.h"
#include "HandlerFactory.h"
#include "CommandRouter.h" // 你的 CommandRouter 类

int main() {
    // 1. 初始化 DI 容器：按依赖顺序注册核心组件（仅需写一次）
    DI_REGISTER_SINGLETON(DBConnection); // 注册 DB 连接（单例）
    DI_REGISTER_SINGLETON(UserDAO, DI_RESOLVE(DBConnection)); // 注册 UserDAO（依赖 DB）
    DI_REGISTER_SINGLETON(UserService, DI_RESOLVE(UserDAO)); // 注册 UserService（依赖 DAO）

    // 2. 创建 CommandRouter 并注册所有 Handler（新增 Handler 仅需加一行）
    CommandRouter commandRouter;
    commandRouter.registerHandler(HandlerFactory::createUserRegisterHandler());
    // commandRouter.registerHandler(HandlerFactory::createUserLoginHandler()); // 新增 Handler

    return 0;
}