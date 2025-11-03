/******************************************************************************
 * Copyright Notice:
 * This file is part of the {your project name, e.g., XF_LIMS} system, owned by {your name/company, e.g., YanKepu}.
 * Unauthorized reproduction, modification, or distribution of this file is prohibited.
 ******************************************************************************
 * File Information:
 * Filename: {current filename, e.g., SampleService.cpp}
 * Module: {module path, e.g., service (business layer)/Sample Management Module}
 * Description:
 *  main.cpp 作为程序入口，核心职责是 
 * 串联所有模块，完成系统初始化→启动服务→优雅退出 的全生命周期管理，
 * 需要协调配置加载、日志初始化、数据库连接、服务实例创建、网络服务启动等关键步骤，
 * 同时处理程序退出信号以释放资源。
 * Dependent Files:
 *   {Directly dependent headers, e.g., "service/ISampleService.h", "dao/SampleDAO.h", "common/Logger.h"}
 ******************************************************************************
 * Created by: YanKepu
 * Creation Date:  2025-11-03
 * Contact:  yankepu@163.com
 ******************************************************************************
 * Modification History:
 * No.   Date        Modifier    Modification (brief description)
 * 1     2025-11-06  ZhangSan    Added sample status validation logic; fixed null pointer issue during registration
 * 2     {date}      {modifier}  {modification content}
 * ...  (Append subsequent modifications here)
 ******************************************************************************/


/* system header */

#include <signal.h>
#include <iostream>

/* lib header */
#include "network/TcpServer.h"
#include "common/Config.h"
#include "common/Logger.h"


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
        common::Config& config = Config::getInstance();     // 获取一个单例模式
        if(!config.load("config/server.conf"))
        {
            std::cerr << "配置文件加载失败！ 路径 config/server.conf" << std::endl;
            return -1;
        }

        common::Logger::getLogger(); // 初始化日志

        // 2. 注册退出信号处理
        signal(SIGINT, handleSignal);
        signal(SIGTERM, handleSignal);

        // 3. 启动TCP服务器
        TcpServer server;   // 创建一个实例
        g_server = &server;     // 给全局的实例赋值
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