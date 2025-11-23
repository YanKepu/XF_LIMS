#include "network/TcpServer.h"




TcpServer::~TcpServer(){
    stop();
};

TcpServer::TcpServer() : 
    server(    // 初始化 server 成员（调用 Poco::Net::TCPServer 的构造函数）
        new Poco::Net::TCPServerConnectionFactoryImpl<MessageHandler>(),  // 连接工厂
        Poco::Net::ServerSocket(common::Config::getInstance().getServerPort(),
        createServerParams()
    )   // 绑定端口的套接字
) 
{
    // 构造函数体：配置线程池
    
}

void TcpServer::start() 
{
    server.start(); 
        
    // common::Logger::getLogger().information("TCP服务器启动，端口: %d", common::Config::getInstance().getServerPort());
}

void TcpServer::stop() 
{

    server.stop();

    
    // common::Logger::getLogger().information("TCP服务器停止");
}

Poco::Net::TCPServerParams* TcpServer::createServerParams(){
    auto& config = common::Config::getInstance();
    Poco::Net::TCPServerParams* params = new Poco::Net::TCPServerParams();

    params->setMaxThreads(config.getInt("Server.max_threads", 16));  // 最大线程数
    // params->setMinThreads(config.getInt("Server.min_threads", 1));
}