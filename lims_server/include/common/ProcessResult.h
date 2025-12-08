
#ifndef __PROCESSRESULT_H__
#define __PROCESSRESULT_H__

#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct ProcessResult {
    bool successflag;
    int code;
    std::string msg;
    json data;
};

#endif