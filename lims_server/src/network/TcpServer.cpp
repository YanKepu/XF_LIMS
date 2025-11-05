

#include "TcpServer.h"

TcpServer::TcpServer() : server(    // 初始化 server 成员（调用 Poco::Net::TCPServer 的构造函数）
    new Poco::Net::TCPServerConnectionFactoryImpl<MessageHandler>(),  // 连接工厂
    Poco::Net::ServerSocket(Config::getInstance().getServerPort())   // 绑定端口的套接字
) 
{
    // 构造函数体：配置线程池
    server.setMaxThreads(10);
}

void start() 
{
    server.start();
    Logger::getLogger().information("TCP服务器启动，端口: %d", Config::getInstance().getServerPort());
}

void stop() 
{
    server.stop();
    Logger::getLogger().information("TCP服务器停止");
}