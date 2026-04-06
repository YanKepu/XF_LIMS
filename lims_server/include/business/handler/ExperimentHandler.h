#ifndef EXPERIMENT_HANDLER_H
#define EXPERIMENT_HANDLER_H

#include "business/abstract/ICommandHandler.h"
#include "service/ExperimentService.h"
#include "common/ProcessResult.h"

namespace business::handler {

class ExperimentHandler : public business::abstract::ICommandHandler {
private:
    ProcessResult handleCreate(const json& reqData, json& response);
    ProcessResult handleList(const json& reqData, json& response);
    ProcessResult handleUpdate(const json& reqData, json& response);
    ProcessResult handleDelete(const json& reqData, json& response);

public:
    ExperimentHandler();
    std::string getCmd() const override;
    ProcessResult handle(const json& reqData, json& response) override;
};

} // namespace business::handler

#endif // EXPERIMENT_HANDLER_H