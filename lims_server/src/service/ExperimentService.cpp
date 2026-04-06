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

nlohmann::json ExperimentService::getExperimentList(const std::string& searchTerm, int page, int pageSize) {
    common::Logger::getLogger().information("ExperimentService", "Getting experiment list, search: " + searchTerm + ", page: " + std::to_string(page) + ", pageSize: " + std::to_string(pageSize));
    return ExperimentDAO::instance().getExperimentList(searchTerm, page, pageSize);
}

int ExperimentService::getExperimentCount(const std::string& searchTerm) {
    common::Logger::getLogger().information("ExperimentService", "Getting experiment count, search: " + searchTerm);
    return ExperimentDAO::instance().getExperimentCount(searchTerm);
}

bool ExperimentService::updateExperiment(const std::string& id, const std::string& name, const std::string& goal,
                                       const std::string& startDate, const std::string& endDate,
                                       const std::string& description, const std::string& status) {
    common::Logger::getLogger().information("ExperimentService", "Updating experiment: " + id);
    return ExperimentDAO::instance().updateExperiment(id, name, goal, startDate, endDate, description, status);
}

bool ExperimentService::deleteExperiment(const std::string& id) {
    common::Logger::getLogger().information("ExperimentService", "Deleting experiment: " + id);
    return ExperimentDAO::instance().deleteExperiment(id);
}
