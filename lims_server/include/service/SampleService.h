#ifndef SAMPLE_SERVICE_H
#define SAMPLE_SERVICE_H

#include <vector>
#include "dao/SampleDAO.h"
#include "models/Sample.h"
#include "common/Logger.h"

class SampleService {
private:
    SampleDAO sampleDAO; // 依赖数据访问层

public:
    // 新增样品（带业务校验）
    nlohmann::json registerSample(const nlohmann::json& data) {
        try {
            // 1. 校验必填字段
            if (!data.contains("code") || !data.contains("name")) {
                return {{"code", 400}, {"msg", "样品编号和名称不能为空"}};
            }

            // 2. 封装样品对象
            Sample sample(
                data["code"].get<std::string>(),
                data["name"].get<std::string>(),
                data["customer"].get<std::string>()
            );

            // 3. 调用DAO保存
            sampleDAO.addSample(sample);
            return {{"code", 200}, {"msg", "样品登记成功"}, {"data", {{"sample_id", sample.code}}}};
        } catch (const std::exception& e) {
            Logger::getLogger().error("样品登记失败: %s", e.what());
            return {{"code", 500}, {"msg", "服务器错误: " + std::string(e.what())}};
        }
    }

    // 获取样品列表
    nlohmann::json getSampleList() {
        try {
            std::vector<Sample> samples = sampleDAO.getSamples();
            nlohmann::json sampleList = nlohmann::json::array();
            for (const auto& s : samples) {
                sampleList.push_back({
                    {"code", s.code},
                    {"name", s.name},
                    {"customer", s.customer},
                    {"status", s.status}
                });
            }
            return {{"code", 200}, {"data", {{"samples", sampleList}}}};
        } catch (const std::exception& e) {
            Logger::getLogger().error("获取样品列表失败: %s", e.what());
            return {{"code", 500}, {"msg", "服务器错误: " + std::string(e.what())}};
        }
    }
};

#endif // SAMPLE_SERVICE_H