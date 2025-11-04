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
#include "common/Config.h"
#include "common/Logger.h"
#include "common/JwtUtil.h"
#include "dao/DBConnection.h"
#include "network/TcpServer.h"
#include <Poco/Exception.h>
#include <signal.h>
#include <iostream>

// 全局退出标志
static volatile bool g_quit = false;

// 信号处理函数
static void handleSignal(int sig) {
    g_quit = true;
    Logger::get().information("Received signal %d, exiting...", sig);
}

int main(int argc, char**argv) {
    try {
        // 步骤1：解析命令行参数（简化：默认配置路径）
        std::string configPath = argc > 1 ? argv[1] : "config/server.conf";

        // 步骤2：初始化配置
        Config::load(configPath);

        // 步骤3：初始化日志
        Logger::init();
        auto& log = Logger::get();
        log.information("LIMS server starting...");

        // 步骤4：初始化数据库连接
        DBConnection::init();

        // 步骤5：初始化JWT工具
        JwtUtil::init();

        // 步骤6：创建并启动TCP服务器
        int port = Config::instance().getInt("TCP.port", 8888);
        int threadPoolSize = Config::instance().getInt("TCP.thread_pool_size", 4);
        TcpServer tcpServer(port, threadPoolSize);
        tcpServer.start();

        // 步骤7：注册退出信号
        signal(SIGINT, handleSignal);
        signal(SIGTERM, handleSignal);

        // 步骤8：阻塞等待退出信号
        while (!g_quit) {
            Poco::Thread::sleep(1000);
        }

        // 步骤9：优雅退出
        tcpServer.stop();
        log.information("LIMS server exited successfully");
        return 0;

    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
}