#ifndef __SAMPLE_H__
#define __SAMPLE_H__
#include <string>
#include <cassert>


class Sample {
private:
    std::string _code;      // 样品编号
    std::string _name;      // 样品名称      
    std::string _customer;  // 客户名称
    std::string _status;    // 样品状态

public:
    // 私有化默认构造函数，防止无效对象创建
    Sample(std::string code = "01", std::string name = "Sample", 
        std::string customer = "Default", std::string status = "New")
        : _code(code), _name(name), _customer(customer), _status(status) {

        assert(!code.empty() && !name.empty());     //简单校验
    }


    void setCode(const std::string &code) {
        assert(!code.empty());
        _code = code;
    }

    std::string getCode() const { return _code; }
    std::string getName() const { return _name; }
    std::string getCustomer() const { return _customer; }
    std::string getStatus() const { return _status; }
};






#endif // __SAMPLE_H__