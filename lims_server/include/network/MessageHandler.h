#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H

#include <Poco/Net/TCPServerConnection.h>
#include <Poco/Net/StreamSocket.h>
#include <Poco/Timespan.h>
#include <nlohmann/json.hpp>
#include "service/SampleService.h"
#include "common/Logger.h"
#include "common/ProcessResult.h"

using json = nlohmann::json;

#define BUFFER_SIZE 4096  // 增大缓冲区（JSON 数据可能更长）



class MessageHandler : public Poco::Net::TCPServerConnection {
public:
    // -------------------------- 必选：构造函数 --------------------------
    // 必须接收 const Poco::Net::StreamSocket& 参数，传递给基类构造
    // explicit 禁止隐式转换，避免意外类型转换
    explicit MessageHandler(const Poco::Net::StreamSocket& socket);

    // -------------------------- 必选：重写 run() 方法 --------------------------
    // 基类中 run() 是纯虚函数（virtual void run() = 0），子类必须实现
    // Poco 会为每个客户端连接创建独立线程，线程入口就是这个 run() 方法
    void run() override;

    // -------------------------- 可选：析构函数 --------------------------
    // 若子类有动态内存分配、资源持有（如文件句柄、锁），需显式声明析构函数
    // 基类 TCPServerConnection 的析构函数是虚函数，子类析构会自动调用
    ~MessageHandler() override;


private:
    // -------------------------- 可选：成员变量（根据业务需求添加） --------------------------
    Poco::Timespan _readTimeout;          // 读超时时间（如 30 秒）
    Poco::Timespan _writeTimeout;         // 写超时时间（如 10 秒）

    // -------------------------- 可选：辅助方法（拆分 run() 逻辑，提高可读性） --------------------------
    // 示例1：处理接收到的客户端数据（业务逻辑核心）
    ProcessResult processData(const json& reqJson, json& response);

    // 示例2：从客户端读取数据（封装读操作，处理超时/断开）
    std::string readFromClient();

    // 示例3：向客户端发送响应（封装写操作，处理超时/失败）
    bool sendToClient(const std::string& response);
};

#endif // MESSAGE_HANDLER_H