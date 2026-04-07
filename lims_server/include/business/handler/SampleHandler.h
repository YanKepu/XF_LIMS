#ifndef SAMPLE_HANDLER_H
#define SAMPLE_HANDLER_H

#include "business/abstract/ICommandHandler.h"
#include "service/SampleService.h"
#include "common/ProcessResult.h"

namespace business::handler {

class SampleHandler : public business::abstract::ICommandHandler {
private:
    ProcessResult handleRegister(const json& reqData, json& response);
    ProcessResult handleList(const json& reqData, json& response);

public:
    SampleHandler();
    std::string getCmd() const override;
    ProcessResult handle(const json& reqData, json& response) override;
};

} // namespace business::handler

#endif // SAMPLE_HANDLER_H
