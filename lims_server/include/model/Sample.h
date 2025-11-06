#ifndef __SAMPLE_H__
#define __SAMPLE_H__
#include <string>
#include <cassert>

namespace model {

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

        Sample() = default; // 默认构造

        void setCode(const std::string &code) {
            assert(!code.empty());
            _code = code;
        }

    }
}





#endif // __SAMPLE_H__