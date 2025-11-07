
#include "service/SampleService.h"

nlohmann::json SampleService::registerSample(const nlohmann::json& data) {
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
        return {{"code", 200}, {"msg", "样品登记成功"}, {"data", {"sample_id", sample.getCode()}}};
    } catch (const std::exception& e) {
        common::Logger::getLogger().error("样品登记失败: %s", e.what());
        return {{"code", 500}, {"msg", "服务器错误: " + std::string(e.what())}};
    }
}

nlohmann::json SampleService::getSampleList() {
    try {
        std::vector<Sample> samples = sampleDAO.getSamples();
        nlohmann::json sampleList = nlohmann::json::array();
        for (const auto& s : samples) {
            sampleList.push_back({
                {"code", s.getCode()},
                {"name", s.getName()},
                {"customer", s.getCustomer()},
                {"status", s.getStatus()}
            });
        }
        return {{"code", 200}, {"data", {{"samples", sampleList}}}};
    } catch (const std::exception& e) {
        common::Logger::getLogger().error("获取样品列表失败: %s", e.what());
        return {{"code", 500}, {"msg", "服务器错误: " + std::string(e.what())}};
    }
}