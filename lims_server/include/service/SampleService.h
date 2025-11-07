#ifndef SAMPLE_SERVICE_H
#define SAMPLE_SERVICE_H

#include <vector>
#include <nlohmann/json.hpp>
#include "dao/SampleDAO.h"
#include "model/Sample.h"
#include "common/Logger.h"
#include "common/Logger.h"

class SampleService {
private:
    SampleDAO sampleDAO; // 依赖数据访问层

public:
    // 新增样品（带业务校验）
    nlohmann::json registerSample(const nlohmann::json& data);

    // 获取样品列表
    nlohmann::json getSampleList();
};

#endif // SAMPLE_SERVICE_H