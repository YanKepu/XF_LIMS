#include "network/TcpServer.h"




TcpServer::~TcpServer(){
    stop();
};

TcpServer::TcpServer() : 
    server(    // 初始化 server 成员（调用 Poco::Net::TCPServer 的构造函数）
        new Poco::Net::TCPServerConnectionFactoryImpl<MessageHandler>(),  // 连接工厂
        Poco::Net::ServerSocket(common::Config::getInstance().getServerPort()),
        createServerParams()
    )
{
    // 构造函数体：配置线程池
    
}

void TcpServer::start() {
    try {
        server.start();
        common::Logger::getLogger().information(
            "TCP服务器启动，端口: %d", common::Config::getInstance().getServerPort()
        ); 
    } catch (Poco::Exception& e) {
        common::Logger::getLogger().error(
            "TCP服务器启动失败: %s", e.displayText()
        );
        throw;
    }
}

void TcpServer::stop() 
{

    server.stop();

    
    // common::Logger::getLogger().information("TCP服务器停止");
}

Poco::Net::TCPServerParams* TcpServer::createServerParams(){
    auto& config = common::Config::getInstance();       // 获取配置实例
    Poco::Net::TCPServerParams* params = new Poco::Net::TCPServerParams();

    params->setMaxThreads(config.getInt("Server.max_threads", 16));  // 最大线程数
    params->setMaxQueued(100); // 最大排队连接数100

    return params;
}

