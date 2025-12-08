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

#include <Poco/Exception.h>
#include <signal.h>
#include <iostream>

#include "common/Config.h"
#include "common/Logger.h"
#include "common/JwtUtil.h"
#include "dao/DBConnection.h"
#include "network/TcpServer.h"
#include "business/CommandRouter.h"
#include "business/abstract/ICommandHandler.h"
#include "business/handler/UserLoginHandler.h"

// 全局退出标志
static volatile bool g_quit = false;

// 信号处理函数
static void handleSignal(int sig) {
    g_quit = true;  //  全局直接设置退出标志
    common::Logger::getLogger().information("Received signal %d, exiting...", sig);       // 日志记录
}

int main(int argc, char**argv) {
    try {
        // 步骤1：解析命令行参数（简化：默认配置路径）
        std::cout << "Starting LIMS Server..." << std::endl;
        std::cout << "Loading configuration..." << std::endl;
        std::string configPath = argc > 1 ? argv[1] : "/home/ubuntu/XF_LIMS/lims_server/config/server.conf";     // 给于一个默认配置

        // 步骤2：初始化配置
        std::cout << "Initializing configuration from " + configPath << std::endl;
        common::Config& config = common::Config::getInstance();     // 单例模式：只有一个实例
        config.load(configPath);            // 新的config是一个私有成员变量，是一个智能指针，自动管理内存
        std::cout << "Configuration loaded." << std::endl;

        // 步骤3：初始化日志  日志先不进行实现，进行其他实现

        // 2. 🔥 主动注册所有业务处理器（顺序可控，无依赖风险）
        business::CommandRouter::instance().registerHandler<business::handler::UserLoginHandler>();

        // 新增模块时，只需加一行注册代码：
        // business::CommandRouter::instance().registerHandler<business::handler::OrderQueryHandler>();

        common::Logger::getLogger().information("所有业务处理器注册完成");


        // 步骤4：初始化数据库连接
        std::cout << "Initializing database connection..." << std::endl;
        DBConnection::init();       /* 调用了config的单例模式，获取配置进行数据库连接 */
        std::cout << "Database connection initialized." << std::endl;

        // 步骤5：初始化JWT工具
        // JwtUtil::init();     先不用Jwt

        // 步骤6：创建并启动TCP服务器
        std::cout << "TCP server create ..." << std::endl;
        common::Logger::getLogger().information(
            "TCP服务器启动，端口: %d", common::Config::getInstance().getServerPort()
        ); 
        TcpServer tcpServer;
        std::cout << "Starting TCP server..." << std::endl;
        tcpServer.start();      // 启动服务器
        std::cout << "TCP server started." << std::endl;

        // 步骤7：注册退出信号
        std::cout << "Registering signal handlers..." << std::endl;
        signal(SIGINT, handleSignal);   // signal 函数用于设置信号处理函数
        signal(SIGTERM, handleSignal);
        std::cout << "Signal handlers registered." << std::endl;

        // 步骤8：阻塞等待退出信号
        while (!g_quit) {
            Poco::Thread::sleep(1000);  // 每秒检查一次退出标志，不接续占用CPU
        }

        // 步骤9：优雅退出
        std::cout << "Shutting down TCP server..." << std::endl;
        tcpServer.stop();
        std::cout << "TCP server shut down." << std::endl;
        std::cout << "LIMS Server exited gracefully." << std::endl;
        return 0;

    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
}