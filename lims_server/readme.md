# server模块开发


## 逻辑分层
| 层 |	核心职责  |	技术依赖 |
| ---| -- | -- |
|通信层（Network）|	处理 TCP 连接、解析 JSON 消息、分发请求 |	POCO Network（TCP 服务器）、nlohmann/json（JSON 解析）|
|业务逻辑层（Service）|	实现核心业务（登录、样品管理、任务分配等） |	无第三方依赖，纯 C++ 逻辑 |
|数据访问层（DAO）|	封装数据库操作（PostgreSQL）	|libpqxx（PostgreSQL C++ 客户端）|
|公共模块（Common） |	工具类（日志、配置、JWT 认证等）|	POCO Util（配置）、libjwt（令牌生成）|

## 代码架构
lims_server/
├── include/                  # 头文件
│   ├── network/              # 通信层头文件
│   │   ├── TcpServer.h       # TCP服务器类
│   │   └── MessageHandler.h  # 消息处理器
│   ├── service/              # 业务逻辑层头文件
│   │   ├── AuthService.h     # 认证服务
│   │   ├── SampleService.h   # 样品服务
│   │   └── TaskService.h     # 任务服务
│   ├── dao/                  # 数据访问层头文件
│   │   ├── UserDAO.h         # 用户数据操作
│   │   ├── SampleDAO.h       # 样品数据操作
│   │   └── DBConnection.h    # 数据库连接封装
│   └── common/               # 公共模块头文件
│       ├── Config.h          # 配置读取
│       ├── Logger.h          # 日志工具
│       └── JwtUtil.h         # JWT令牌工具
├── src/                      # 源文件（对应include目录）
|   ├── network/               # 通信层实现
|   │   ├── TcpServer.cpp      # TCP服务器实现，处理连接管理
    │   └── MessageHandler.cpp # 消息处理器实现，解析请求并分发到对应服务
    ├── service/               # 业务逻辑层实现
    │   ├── AuthService.cpp    # 认证服务实现（登录、权限验证等）
    │   ├── SampleService.cpp  # 样品服务实现（样品登记、状态更新等）
    │   └── TaskService.cpp    # 任务服务实现（任务创建、分配、跟踪等）
    ├── dao/                   # 数据访问层实现
    │   ├── UserDAO.cpp        # 用户数据操作实现
    │   ├── SampleDAO.cpp      # 样品数据操作实现
    │   └── DBConnection.cpp   # 数据库连接管理实现
    ├── common/                # 公共模块实现
    │   ├── Config.cpp         # 配置读取实现（从server.conf加载配置）
    │   ├── Logger.cpp         # 日志工具实现（文件日志、控制台日志）
    │   └── JwtUtil.cpp        # JWT令牌工具实现（生成、验证令牌）
└── main.cpp               # 程序入口，初始化各模块并启动服务
├── config/                   # 配置文件
│   └── server.conf           # 服务器配置（端口、数据库地址等）
├── sql/                      # 数据库初始化脚本
│   └── init_db.sql           # 创建表结构
├── CMakeLists.txt            # 编译配置
└── main.cpp                  # 程序入口

## 基础开发工具
工具 / 环境	用途	安装命令（apt）	
g++/gcc	C++ 编译器	sudo apt install g++	推荐 9.4.0 及以上版本
cmake	项目构建工具（管理编译流程）	sudo apt install cmake	用于解析 CMakeLists.txt，生成 Makefile
git	版本控制（可选）	sudo apt install git	管理代码版本
ssh	远程连接服务器（开发必备）	sudo apt install openssh-server	本地通过 SSH 连接 Linux 服务器开发
vim/nano	终端文本编辑器	sudo apt install vim	编辑代码和配置文件

## 核心系统三方库
库名 / 软件	用途	安装命令（apt）	备注
PostgreSQL	关系型数据库	sudo apt install postgresql postgresql-contrib	需初始化数据库并创建用户 / 表
libpqxx-dev	PostgreSQL 的 C++ 客户端库	sudo apt install libpqxx-dev	服务端通过它操作 PostgreSQL
libpoco-dev	C++ 轻量级框架（网络 / 配置等）	sudo apt install libpoco-dev	提供 TCP 服务器、配置解析等核心功能
nlohmann-json3	C++ JSON 解析库（头文件）	sudo apt install nlohmann-json3-dev	解析客户端发送的 JSON 消息
libjwt-dev	JWT 令牌生成 / 验证库（可选）	sudo apt install libjwt-dev	用于用户认证的令牌生成（后期扩展用）
valgrind	内存泄漏检测工具（调试用）	sudo apt install valgrind	开发阶段检测内存问题
tcpdump	网络抓包工具（调试用）	sudo apt install tcpdump	排查客户端 - 服务端通信问题

## 个人开发问题
* Q1 为什么C++中常用try catch风格的代码，与C语言中的返回值判断有何差异。
* A1 C语言的返回值判断有4个痛点。 1、容易忽略错误码判断   2、错误码需要逐层传递，容易断裂  3、需要额外维护错误码对照表
        try  catch是强制性的，而且可以抓住构造和析构函数的错误
        但是错误值判断在C++中仍然有用处，例如在轻量级代码中，或者性能敏感性代码中常用，try-catch有多余的开销。
        其次是在错误可以预判值的情况下可以使用。另外在使用C兼容库中可能会涉及。