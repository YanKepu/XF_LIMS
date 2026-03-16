#ifndef EXPERIMENT_DAO_H
#define EXPERIMENT_DAO_H

#include <string>

class ExperimentDAO {
public:
    static ExperimentDAO& instance(); // Singleton accessor

    bool createExperiment(const std::string& id, const std::string& name, const std::string& goal,
                          const std::string& startDate, const std::string& endDate,
                          const std::string& description, const std::string& status);

private:
    ExperimentDAO() = default; // Private constructor for singleton
    ExperimentDAO(const ExperimentDAO&) = delete;
    ExperimentDAO& operator=(const ExperimentDAO&) = delete;
};

#endif // EXPERIMENT_DAO_H
