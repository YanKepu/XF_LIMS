#ifndef __USERLOGINHANDLER_H__
#define __USERLOGINHANDLER_H__
#include "business/abstract/ICommandHandler.h"
#include "common/ProcessResult.h"

namespace business {
    namespace handler {

        class UserLoginHandler : public business::abstract::ICommandHandler {
        public:
            std::string getCmd() const override;  // 绑定 "user_login"
            ProcessResult handle(const json& reqData, json& response) override;  // 登录业务逻辑
        };

    } // namespace handler
} // namespace business
#endif