#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H

#include <Poco/Net/TCPServerConnection.h>
#include <Poco/Net/StreamSocket.h>
#include <nlohmann/json.hpp>
#include "service/SampleService.h"
#include "common/Logger.h"

using json = nlohmann::json;

class MessageHandler : public Poco::Net::TCPServerConnection {
private:
    // 业务服务实例
    AuthService authService;
    SampleService sampleService;

    // 解析客户端消息
    json parseMessage(Poco::Net::StreamSocket& socket) {
        std::string msg;
        char buffer[1024];
        int n = socket.receiveBytes(buffer, sizeof(buffer)-1);
        if (n <= 0) {
            throw std::runtime_error("接收消息失败");
        }
        buffer[n] = '\0';
        msg = buffer;
        return json::parse(msg); // 解析JSON
    }

    // 处理消息并返回响应
    json handleRequest(const json& request) {
        if (!request.contains("type")) {
            return {{"code", 400}, {"msg", "缺少消息类型"}};
        }

        std::string type = request["type"];
        // 根据消息类型分发到对应业务服务
        if (type == "user_login") {
            return authService.login(request["data"]);
        } else if (type == "sample_register") {
            return sampleService.registerSample(request["data"]);
        } else if (type == "sample_list") {
            return sampleService.getSampleList();
        } else {
            return {{"code", 404}, {"msg", "未知消息类型"}};
        }
    }

public:
    explicit MessageHandler(const Poco::Net::StreamSocket& socket) : TCPServerConnection(socket) {}

    // 处理连接（POCO框架回调）
    void run() override {
        Poco::Net::StreamSocket& socket = this->socket();
        try {
            // 1. 读取客户端消息
            json request = parseMessage(socket);
            Logger::getLogger().information("收到消息: %s", request.dump());

            // 2. 处理消息
            json response = handleRequest(request);
            response["type"] = request["type"]; // 响应带回消息类型

            // 3. 发送响应
            std::string responseStr = response.dump() + "\n";
            socket.sendBytes(responseStr.data(), responseStr.size());
        } catch (const std::exception& e) {
            Logger::getLogger().error("处理连接失败: %s", e.what());
            json errorResp = {{"code", 500}, {"msg", e.what()}};
            socket.sendBytes(errorResp.dump().data(), errorResp.dump().size());
        }
    }
};

#endif // MESSAGE_HANDLER_H