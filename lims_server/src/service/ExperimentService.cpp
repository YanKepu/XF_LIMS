#include "../include/service/ExperimentService.h"
#include "common/Logger.h"
#include "Poco/UUIDGenerator.h"
#include "dao/ExperimentDAO.h" // Include ExperimentDAO for instance access

ExperimentService& ExperimentService::instance() {
    static ExperimentService instance;
    return instance;
}

std::string ExperimentService::createExperiment(const std::string& name, const std::string& goal,
                                             const std::string& startDate, const std::string& endDate,
                                             const std::string& description, const std::string& status) {
    Poco::UUIDGenerator uuidGen;
    std::string id = uuidGen.create().toString(); // Generate a unique ID

    if (ExperimentDAO::instance().createExperiment(id, name, goal, startDate, endDate, description, status)) {
        common::Logger::getLogger().information("ExperimentService", "Experiment created in DB: " + name);
        return id;
    } else {
        common::Logger::getLogger().error("ExperimentService", "Failed to create experiment in DB: " + name);
        return ""; // Indicate failure
    }
}
