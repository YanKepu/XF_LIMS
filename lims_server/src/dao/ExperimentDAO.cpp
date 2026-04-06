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
        auto conn_ptr = DBConnection::getConnection();
        if (!conn_ptr) {
            common::Logger::getLogger().critical("ExperimentDAO", "Database connection is not initialized!");
            return false;
        }
        pqxx::connection& conn = *conn_ptr; // Get connection via static method
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

nlohmann::json ExperimentDAO::getExperimentList(const std::string& searchTerm, int page, int pageSize) {
    nlohmann::json experiments = nlohmann::json::array();
    try {
        auto conn_ptr = DBConnection::getConnection();
        if (!conn_ptr) {
            common::Logger::getLogger().critical("ExperimentDAO", "Database connection is not initialized!");
            return experiments;
        }
        pqxx::connection& conn = *conn_ptr;
        pqxx::work txn(conn);

        std::string sql = "SELECT id, name, status, start_date, end_date FROM experiments";
        if (!searchTerm.empty()) {
            sql += " WHERE name ILIKE $1 OR goal ILIKE $1";
        }
        sql += " ORDER BY created_at DESC LIMIT $" + std::to_string(searchTerm.empty() ? 1 : 2) + 
               " OFFSET $" + std::to_string(searchTerm.empty() ? 2 : 3);

        pqxx::result res;
        if (!searchTerm.empty()) {
            std::string searchPattern = "%" + searchTerm + "%";
            res = txn.exec_params(sql, searchPattern, pageSize, (page - 1) * pageSize);
        } else {
            res = txn.exec_params(sql, pageSize, (page - 1) * pageSize);
        }

        for (const auto& row : res) {
            nlohmann::json experiment;
            experiment["id"] = row["id"].as<std::string>();
            experiment["name"] = row["name"].as<std::string>();
            experiment["status"] = row["status"].as<std::string>();
            experiment["start_date"] = row["start_date"].as<std::string>();
            experiment["end_date"] = row["end_date"].as<std::string>();
            experiments.push_back(experiment);
        }

        return experiments;
    } catch (const pqxx::sql_error& e) {
        common::Logger::getLogger().error("ExperimentDAO", "SQL Error getting experiment list: " + std::string(e.what()));
        return experiments;
    } catch (const std::exception& e) {
        common::Logger::getLogger().error("ExperimentDAO", "Error getting experiment list: " + std::string(e.what()));
        return experiments;
    }
}

int ExperimentDAO::getExperimentCount(const std::string& searchTerm) {
    try {
        auto conn_ptr = DBConnection::getConnection();
        if (!conn_ptr) {
            common::Logger::getLogger().critical("ExperimentDAO", "Database connection is not initialized!");
            return 0;
        }
        pqxx::connection& conn = *conn_ptr;
        pqxx::work txn(conn);

        std::string sql = "SELECT COUNT(*) FROM experiments";
        if (!searchTerm.empty()) {
            sql += " WHERE name ILIKE $1 OR goal ILIKE $1";
        }

        pqxx::result res;
        if (!searchTerm.empty()) {
            std::string searchPattern = "%" + searchTerm + "%";
            res = txn.exec_params(sql, searchPattern);
        } else {
            res = txn.exec(sql);
        }

        return res[0][0].as<int>();
    } catch (const pqxx::sql_error& e) {
        common::Logger::getLogger().error("ExperimentDAO", "SQL Error getting experiment count: " + std::string(e.what()));
        return 0;
    } catch (const std::exception& e) {
        common::Logger::getLogger().error("ExperimentDAO", "Error getting experiment count: " + std::string(e.what()));
        return 0;
    }
}

bool ExperimentDAO::updateExperiment(const std::string& id, const std::string& name, const std::string& goal,
                                   const std::string& startDate, const std::string& endDate,
                                   const std::string& description, const std::string& status) {
    try {
        auto conn_ptr = DBConnection::getConnection();
        if (!conn_ptr) {
            common::Logger::getLogger().critical("ExperimentDAO", "Database connection is not initialized!");
            return false;
        }
        pqxx::connection& conn = *conn_ptr;
        pqxx::work txn(conn);

        std::string sql = "UPDATE experiments SET "
                          "name = $2, goal = $3, start_date = $4, end_date = $5, description = $6, status = $7 "
                          "WHERE id = $1";
        pqxx::result res = txn.exec_params(sql, id, name, goal, startDate, endDate, description, status);
        txn.commit();

        return res.affected_rows() > 0;
    } catch (const pqxx::sql_error& e) {
        common::Logger::getLogger().error("ExperimentDAO", "SQL Error updating experiment: " + std::string(e.what()));
        return false;
    } catch (const std::exception& e) {
        common::Logger::getLogger().error("ExperimentDAO", "Error updating experiment: " + std::string(e.what()));
        return false;
    }
}

bool ExperimentDAO::deleteExperiment(const std::string& id) {
    try {
        auto conn_ptr = DBConnection::getConnection();
        if (!conn_ptr) {
            common::Logger::getLogger().critical("ExperimentDAO", "Database connection is not initialized!");
            return false;
        }
        pqxx::connection& conn = *conn_ptr;
        pqxx::work txn(conn);

        std::string sql = "DELETE FROM experiments WHERE id = $1";
        pqxx::result res = txn.exec_params(sql, id);
        txn.commit();

        return res.affected_rows() > 0;
    } catch (const pqxx::sql_error& e) {
        common::Logger::getLogger().error("ExperimentDAO", "SQL Error deleting experiment: " + std::string(e.what()));
        return false;
    } catch (const std::exception& e) {
        common::Logger::getLogger().error("ExperimentDAO", "Error deleting experiment: " + std::string(e.what()));
        return false;
    }
}
