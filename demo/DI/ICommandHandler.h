// 示例：ICommandHandler.h（所有 Handler 的基类）
#pragma once
class ICommandHandler {
public:
    virtual ~ICommandHandler() = default;
    virtual void handle() = 0;
};

// 示例：UserRegisterHandler.h
#pragma once
#include "ICommandHandler.h"
#include "UserService.h"
#include <memory>
class UserRegisterHandler : public ICommandHandler {
public:
    // 构造传参（保留测试友好性，Mock 时可传假对象）
    UserRegisterHandler(std::shared_ptr<UserService> userService) : _userService(userService) {}
    void handle() override {
        _userService->registerUser("test_user");
    }
private:
    std::shared_ptr<UserService> _userService;
};

