#include "../include/dao/ExperimentDAO.h"
#include "common/Logger.h"
#include "dao/DBConnection.h" // Include DBConnection
#include <pqxx/pqxx>

ExperimentDAO& ExperimentDAO::instance() {
    static ExperimentDAO instance;
    return instance;
}

bool ExperimentDAO::createExperiment(const std::string& id, const std::string& name, const std::string& goal,
                                  const std::string& startDate, const std::string& endDate,
                                  const std::string& description, const std::string& status) {
    try {
        pqxx::connection& conn = *DBConnection::getConnection(); // Get connection via static method
        pqxx::work txn(conn); // Start a transaction

        std::string sql = "INSERT INTO experiments (id, name, goal, start_date, end_date, description, status) "
                          "VALUES ($1, $2, $3, $4, $5, $6, $7)";
        txn.exec_params(sql, id, name, goal, startDate, endDate, description, status);
        txn.commit(); // Commit the transaction
        common::Logger::getLogger().information("ExperimentDAO", "Experiment '" + name + "' inserted into database.");
        return true;
    } catch (const pqxx::sql_error& e) {
        common::Logger::getLogger().error("ExperimentDAO", "SQL Error creating experiment: " + std::string(e.what()));
        return false;
    } catch (const std::exception& e) {
        common::Logger::getLogger().error("ExperimentDAO", "Error creating experiment: " + std::string(e.what()));
        return false;
    }
}
