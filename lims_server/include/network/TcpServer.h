#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <Poco/Net/TCPServer.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/TCPServerConnectionFactory.h>
#include <Poco/Net/TCPServerParams.h>
#include "MessageHandler.h"
#include "common/Config.h"

class TcpServer {
private:
    Poco::Net::TCPServer server;        // 成员变量

public:
    TcpServer();        // 构造函数
    ~TcpServer();       // 析构函数

    void start();
    void stop();
};

#endif // TCP_SERVER_H