#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <Poco/Net/TCPServer.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/TCPServerConnectionFactory.h>
#include "MessageHandler.h"
#include "common/Config.h"

class TcpServer {
private:
    Poco::Net::TCPServer server;

public:
    TcpServer() : server(new Poco::Net::TCPServerConnectionFactoryImpl<MessageHandler>(), 
                         Poco::Net::ServerSocket(Config::getInstance().getServerPort())) {
        // 配置线程池（最多10个线程处理并发连接）
        server.setMaxThreads(10);
    }

    void start() {
        server.start();
        Logger::getLogger().information("TCP服务器启动，端口: %d", Config::getInstance().getServerPort());
    }

    void stop() {
        server.stop();
        Logger::getLogger().information("TCP服务器停止");
    }
};

#endif // TCP_SERVER_H