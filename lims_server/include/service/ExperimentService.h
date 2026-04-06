#ifndef EXPERIMENT_SERVICE_H
#define EXPERIMENT_SERVICE_H

#include <string>
#include "nlohmann/json.hpp"
#include "dao/ExperimentDAO.h"

class ExperimentService {
public:
    static ExperimentService& instance(); // Singleton accessor

    std::string createExperiment(const std::string& name, const std::string& goal,
                                 const std::string& startDate, const std::string& endDate,
                                 const std::string& description, const std::string& status);

    nlohmann::json getExperimentList(const std::string& searchTerm, int page, int pageSize);
    int getExperimentCount(const std::string& searchTerm);
    bool updateExperiment(const std::string& id, const std::string& name, const std::string& goal,
                         const std::string& startDate, const std::string& endDate,
                         const std::string& description, const std::string& status);
    bool deleteExperiment(const std::string& id);

private:
    ExperimentService() = default; // Private constructor for singleton
    ExperimentService(const ExperimentService&) = delete;
    ExperimentService& operator=(const ExperimentService&) = delete;
};

#endif // EXPERIMENT_SERVICE_H
