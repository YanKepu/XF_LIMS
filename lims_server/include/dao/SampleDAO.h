#ifndef SAMPLE_DAO_H
#define SAMPLE_DAO_H

#include <pqxx/pqxx>
#include <vector>
#include "model/Sample.h"
#include "dao/DBConnection.h"

class SampleDAO {
public:
    // 新增样品
    bool addSample(const Sample& sample) {
        auto conn = DBConnection::getConnection();
        pqxx::work txn(*conn);
        try {
            txn.exec_params(
                "INSERT INTO samples (code, name, customer, status) VALUES ($1, $2, $3, $4)",
                sample.getCode(), sample.getName(), sample.getCustomer(), sample.getStatus()
            );
            txn.commit();
            return true;
        } catch (const std::exception& e) {
            txn.abort();
            throw std::runtime_error("添加样品失败: " + std::string(e.what()));
        }
    }

    // 查询所有样品
    std::vector<Sample> getSamples() {
        auto conn = DBConnection::getConnection();
        pqxx::nontransaction txn(*conn);
        pqxx::result res = txn.exec("SELECT code, name, customer, status FROM samples");
        
        std::vector<Sample> samples;
        for (const auto& row : res) {
            samples.emplace_back(
                row["code"].as<std::string>(),
                row["name"].as<std::string>(),
                row["customer"].as<std::string>(),
                row["status"].as<std::string>()
            );
        }
        return samples;
    }
};

#endif // SAMPLE_DAO_H