#ifndef EXPERIMENT_DAO_H
#define EXPERIMENT_DAO_H

#include <string>
#include "nlohmann/json.hpp"

class ExperimentDAO {
public:
    static ExperimentDAO& instance(); // Singleton accessor

    bool createExperiment(const std::string& id, const std::string& name, const std::string& goal,
                          const std::string& startDate, const std::string& endDate,
                          const std::string& description, const std::string& status);

    nlohmann::json getExperimentList(const std::string& searchTerm, int page, int pageSize);
    int getExperimentCount(const std::string& searchTerm);
    bool updateExperiment(const std::string& id, const std::string& name, const std::string& goal,
                         const std::string& startDate, const std::string& endDate,
                         const std::string& description, const std::string& status);
    bool deleteExperiment(const std::string& id);

private:
    ExperimentDAO() = default; // Private constructor for singleton
    ExperimentDAO(const ExperimentDAO&) = delete;
    ExperimentDAO& operator=(const ExperimentDAO&) = delete;
};

#endif // EXPERIMENT_DAO_H
