#ifndef EXPERIMENT_CREATE_HANDLER_H
#define EXPERIMENT_CREATE_HANDLER_H

#include "business/abstract/ICommandHandler.h"
#include "service/ExperimentService.h"
#include "common/ProcessResult.h" // Include ProcessResult

class ExperimentCreateHandler : public business::abstract::ICommandHandler {
public:
    ExperimentCreateHandler(); // Default constructor
    std::string getCmd() const override { return "create_experiment"; }
    ProcessResult handle(const json& reqData, json& response) override; // Corrected signature
private:
    // No direct dependency injection in constructor for CommandRouter registration
};

#endif // EXPERIMENT_CREATE_HANDLER_H
