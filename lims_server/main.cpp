#include "network/TcpServer.h"
#include "common/Config.h"
#include "common/Logger.h"
#include <signal.h>
#include <iostream>

// 处理程序退出信号（如Ctrl+C）
static TcpServer* g_server = nullptr;
void handleSignal(int sig) {
    if (g_server) {
        g_server->stop();
    }
    exit(0);
}

int main(int argc, char**argv) {
    try {
        // 1. 初始化配置和日志
        Config::getInstance().load("config/server.conf");
        Logger::getLogger(); // 初始化日志

        // 2. 注册退出信号处理
        signal(SIGINT, handleSignal);
        signal(SIGTERM, handleSignal);

        // 3. 启动TCP服务器
        TcpServer server;
        g_server = &server;
        server.start();

        // 4. 阻塞等待（保持程序运行）
        std::cout << "服务器运行中，按Ctrl+C退出..." << std::endl;
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    } catch (const std::exception& e) {
        std::cerr << "启动失败: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}