#ifndef EXPERIMENT_SERVICE_H
#define EXPERIMENT_SERVICE_H

#include <string>
#include "dao/ExperimentDAO.h"

class ExperimentService {
public:
    static ExperimentService& instance(); // Singleton accessor

    std::string createExperiment(const std::string& name, const std::string& goal,
                                 const std::string& startDate, const std::string& endDate,
                                 const std::string& description, const std::string& status);

private:
    ExperimentService() = default; // Private constructor for singleton
    ExperimentService(const ExperimentService&) = delete;
    ExperimentService& operator=(const ExperimentService&) = delete;
};

#endif // EXPERIMENT_SERVICE_H
