// HandlerFactory.h（封装 Handler 创建逻辑，新增 Handler 仅需加这里）
#pragma once
#include "UserRegisterHandler.h"
#include "DIContainer.h"
#include <memory>

class HandlerFactory {
public:
    // 创建 UserRegisterHandler（从 DI 容器自动获取依赖）
    static std::shared_ptr<UserRegisterHandler> createUserRegisterHandler() {
        return std::make_shared<UserRegisterHandler>(DI_RESOLVE(UserService));
    }

    // 新增 Handler 时，仅需添加对应 create 方法
    // static std::shared_ptr<UserLoginHandler> createUserLoginHandler() {
    //     return std::make_shared<UserLoginHandler>(DI_RESOLVE(UserService));
    // }