
#include <Poco/Exception.h>
#include <Poco/Net/Socket.h>
#include <iostream>
#include "network/MessageHandler.h"
#include "common/JsonUtil.h"
#include "business/CommandRouter.h"



MessageHandler::MessageHandler(const Poco::Net::StreamSocket& socket) 
    : TCPServerConnection(socket),
      _readTimeout(Poco::Timespan(30, 0)),   // 30秒读超时
      _writeTimeout(Poco::Timespan(10, 0))   // 10秒写超时
{
    // 构造函数体（如有需要，可初始化其他成员变量）
}

MessageHandler::~MessageHandler() {
    // 析构函数体（如有需要，释放资源）
}

// 核心：run() 方法实现（线程入口）
void MessageHandler::run() {
    // 获取客户端套接字（基类的 socket() 方法）
    Poco::Net::StreamSocket& clientSocket = socket();
    
    // 打印客户端连接信息（IP:端口）
    std::string clientAddr = clientSocket.peerAddress().toString();
    std::cout << "Client connected: " << clientAddr << std::endl;
    // common::Logger::getLogger().information("客户端连接：%s", clientAddr);

    char buffer[BUFFER_SIZE] = {0};
    try {
        // 循环读取客户端数据，直到连接断开或超时
        while (true) {
            std::cout << "Waiting for data from " << clientAddr << "..." << std::endl;

            // 调用修改后的 readFromClient，获取完整请求（无 '\n'）
            std::string completeReq = readFromClient();
            if (completeReq.empty()) {
                std::cout << "Client " << clientAddr << " disconnected or read timeout." << std::endl;
                break; // 无完整请求，退出循环
            }

            // 打印完整请求（确认接收正确）
            std::cout << "Received complete data from " << clientAddr << ": " << completeReq << std::endl;
            common::Logger::getLogger().debug("收到客户端 %s 完整请求：%s", clientAddr, completeReq.c_str());

            /* 2. 初始化请求 */
            json reqJson;
            json respJson;
            json dataJson;
            try {
                /* 3 json解包： 字符串 -> json */
                reqJson = JsonUtil::parse(completeReq);

                /* 4. 校验必填字段 */
                JsonUtil::checkRequiredFields(reqJson, {"cmd", "data"});

                /* 5. 处理业务逻辑（根据 cmd 调用不同处理函数） */
                ProcessResult processOk = processData(reqJson, respJson);
                
                respJson = JsonUtil::createResponse(processOk.code, processOk.msg, processOk.data);
                std::cout << "回复报文组包完成 " << std::endl;


            }catch (const JsonParseException& e) {
                // JSON 解析错误
                respJson = JsonUtil::createResponse(-1, e.what(), dataJson);
            } catch (const JsonFieldMissingException& e) {
                // 缺失必填字段
                respJson = JsonUtil::createResponse(-32, e.what(), dataJson);
            } catch (const std::exception& e) {
                // 其他异常
                respJson = JsonUtil::createResponse(-99, e.what(), dataJson);
            }

            /* 6. 打包响应： json -> 字符串 */
            std::cout << "开始发送response " << std::endl;
            std::string response = JsonUtil::stringify(respJson) + "\n";
            if (!sendToClient(response)) {
                break;
            }
            std::cout << "response 发送结束" << std::endl;

            

            // 重置缓冲区
            memset(buffer, 0, BUFFER_SIZE);

            // 关键修改1：短连接模式，处理完请求后主动关闭连接，服务端和客户端都断开
            clientSocket.shutdown(); // 关闭读写
            clientSocket.close();
        }
    } catch (Poco::Exception& e) {
        // 捕获 Poco 库抛出的异常（如套接字错误、超时）
        common::Logger::getLogger().error("客户端 %s 处理异常：%s", clientAddr, e.displayText());
    } catch (Poco::TimeoutException& e) {
        // 捕获超时异常
        common::Logger::getLogger().warning("客户端 %s 处理超时：%s", clientAddr, e.displayText());
    }   

    // 连接结束，打印日志
    common::Logger::getLogger().information("客户端 %s 断开连接", clientAddr);
}

// 辅助方法：从客户端读取数据（封装超时逻辑）
std::string MessageHandler::readFromClient() {
    Poco::Net::StreamSocket& clientSocket = socket();
    const int CHUNK_SIZE = 4096;
    char chunk[CHUNK_SIZE] = {0};
    std::string reqBuffer;
    const Poco::Timespan TOTAL_TIMEOUT(10, 0);  // 总超时10s
    const Poco::Timespan SINGLE_TIMEOUT(0, 500000);  // 单次recv超时500ms

    Poco::Timestamp startTime; // 记录开始接收的时间（用于总超时判断）
    startTime.update();

    clientSocket.setReceiveTimeout(SINGLE_TIMEOUT);

    while (true) {
        // 1. 检查总超时：超过10秒没收到完整请求，返回空
        Poco::Timestamp now;
        if ((now - startTime) > TOTAL_TIMEOUT.totalMilliseconds()) {
            std::cout << "超时了！！！！" << std::endl;
            common::Logger::getLogger().warning("读取客户端数据总超时（10秒）");
            return "";
        }
        try
        {
            // 2. 读取一次数据（最多 CHUNK_SIZE-1 字节，留1字节存'\0'）
            ssize_t bytesRead = clientSocket.receiveBytes(chunk, CHUNK_SIZE - 1);
            if (bytesRead > 0) {
                // 3. 将本次读取的 chunk 追加到缓存
                reqBuffer.append(chunk, bytesRead);
                std::cout << "本次收到报文：" << bytesRead << "内容："<< chunk << std::endl;
                // 4. 检查缓存中是否包含结束标记 '\n'（说明请求完整）
                size_t newlinePos = reqBuffer.find('\n');
                if (newlinePos != std::string::npos) {
                    // 截取 '\n' 之前的内容（去掉结束标记，避免 JSON 解析错误）
                    std::string completeReq = reqBuffer.substr(0, newlinePos);
                    // （可选）如果缓存有剩余数据（比如一次发了两个请求），留到下次读取（长连接支持）
                    reqBuffer = reqBuffer.substr(newlinePos + 1);
                    return completeReq; // 返回完整请求
                }
            } else if (bytesRead == 0) {
                // 客户端正常关闭连接，返回空
                common::Logger::getLogger().debug("客户端正常关闭连接");
                return "";
            } 

            
        } catch(const Poco::TimeoutException& e){
            std::cout << "单次recv超时（500ms）：" << e.what() << std::endl;
            common::Logger::getLogger().trace("读取客户端数据临时无数据，继续等待");
        } catch (const Poco::IOException& e) {
            // 连接重置/断开等IO异常，打印并返回
            std::cout << "IO异常（连接断开/重置）：" << e.what() << std::endl;
            common::Logger::getLogger().error("读取客户端数据IO错误：%s", e.what());
            return "";
        } catch(const std::exception& e) {
            std::cerr << "未知异常" << e.what() << '\n';
        }
        // 清空 chunk 缓冲区，准备下次读取
        memset(chunk, 0, CHUNK_SIZE);
        
    }
}

// 辅助方法：向客户端发送响应
bool MessageHandler::sendToClient(const std::string& response) {
    Poco::Net::StreamSocket& clientSocket = socket();
    try {
        clientSocket.setSendTimeout(_writeTimeout);
        clientSocket.sendBytes(response.data(), response.size());
        return true;
    } catch (Poco::TimeoutException&) {
        common::Logger::getLogger().warning("客户端发送超时");
        return false;
    }
}

// 辅助方法：业务逻辑处理（根据你的需求修改）
ProcessResult MessageHandler::processData(const json& reqJson, json& response) {
    ProcessResult result;
    try
    {
        // -------------- 第一步：校验并提取 cmd 和 data --------------
        // 基础校验：确保请求包含 "cmd" 和 "data" 字段（避免后续空指针）
        if (!reqJson.contains("cmd") || !reqJson["cmd"].is_string()) {
            result.successflag = false;
            result.code = 400;
            result.msg = "请求缺少有效 cmd 字段";
            common::Logger::getLogger().error("请求参数异常：%s", reqJson.dump().c_str());
            return result;
        }
        if (!reqJson.contains("data") || !reqJson["data"].is_object()) {
            result.successflag = false;
            result.code = 400;
            result.msg = "请求缺少有效 data 字段";
            common::Logger::getLogger().error("请求参数异常：%s", reqJson.dump().c_str());
            return result;
        }

        /* 从请求Json中获取业务字段（nlohmann::json支持直接[]访问，自动类型转换 */
        std::string cmd = reqJson["cmd"];
        json reqData = reqJson["data"]; // 传给业务接口的具体数据（如 {"username":"admin", "password":"123456"}）

        // -------------- 第二步：调用路由器获取对应的业务处理器 --------------
        // 路由器是单例，通过 cmd 查找绑定的处理器（如 cmd="user_login" → UserLoginHandler）
        auto handler = business::CommandRouter::instance().getHandler(cmd);
        if (!handler) {
            // 未找到处理器（未知 cmd）
            result.successflag = false;
            result.code = 404;
            result.msg = "未知命令：" + cmd;
            common::Logger::getLogger().error("路由器未找到处理器：cmd=%s", cmd.c_str());
            return result;
        }

        // -------------- 第三步：调用业务接口（处理器的 handle 方法） --------------
        // 调用 handle 方法执行具体业务逻辑，传入业务数据 reqData，传出响应 response
        result = handler->handle(reqData, response);

        // -------------- 第四步：返回处理结果 --------------
        common::Logger::getLogger().information("业务处理完成：cmd=%s，结果=%s", cmd.c_str(), result.successflag ? "成功" : "失败");























    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return result;
    }
    
}