#pragma once
#include "business/abstract/ICommandHandler.h"
#include "service/UserService.h"

namespace business {
    namespace handler {
        // 注册命令处理器（TCP命令：REGISTER）
        class UserRegisterHandler : public business::abstract::ICommandHandler {
        private:
            std::shared_ptr<UserService> userService;

        public:
            std::string getCmd() const override;  // 绑定 "user_register"
            
            ProcessResult handle(const json& reqData, json& response) override;

            // UserRegisterHandler(UserService& service) : userService(service) {}
        };
    }
}