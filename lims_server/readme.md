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
    |—— business               # 业务层（抽象+具体业务模块）
    |   |—— abstract            # 抽象层（统一接口层）
    |   |   └── ICommandHandler.h  # 命令处理抽象接口
    |   |—— handler             # 具体业务处理器（实现抽象接口）
    |   ├── UserLoginHandler.h
    │   │   ├── UserRegisterHandler.h
    │   │   └── ...
    │   └── CommandRouter.h    // 路由注册器（依赖抽象接口，不依赖具体处理器）
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


客户端请求 → MessageHandler（网络层）→ UserLoginHandler（business层）→ UserDAO（DAO层）→ 数据库
                                  ↑                ↑                ↑
                                  |                |                |
                           业务逻辑判断     数据读写（无业务）     存储数据

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

* Q2 客户端第一次点击登录时，服务端没有进入处理流程中，客户端的报错是服务器断开连接，第二次点击登录，服务端才进入了数据接收中。
* A1 问题分析。 1、客户端应是发送了数据出去，因为每次的操作对于客户端来说都没有任何的区别。
               怀疑方向1：第一次连接实际上超时了，因此服务端断开了连接。  —— no，因为后续持续的点击也会出现交叉的情况。
               怀疑方向2：客户端多次复用导致状态连接。  —— 不存在，调试阶段是单线程连接
               怀疑方向3：低概率：服务端Handler未重置状态，残留旧数据，Poco 的MessageHandler是 “每连接一个实例”，但如果你的代码中存在 “静态 / 全局的报文缓冲区”，多个 Handler 会共享该缓冲区 → 前一次连接的残留数据污染后一次的登录报文 → 解析出错误密码；若缓冲区为空 → 判定断开。
               怀疑方向4：客户端发送问题，加打印排查此项。后续怀疑是TCPServer的Accept链路阻塞（首次连接未被处理）
               最后定位到TCPServer的原因，readFromClient函数强制依赖\n作为请求结束标记，但客户端发送的登录 JSON 请求里没有包含\n → 服务端会无限循环读取数据，直到 10 秒总超时返回空，导致第一次登录无日志、客户端报断开；第二次登录大概率因「TCP 粘包」（第一次的空数据 + 第二次的请求）或「新连接未到超时就偶然触发结束符」，才侥幸读到完整请求，本质是结束符逻辑与客户端报文不匹配。

               原因后续已经定位，应该是因为Server进入了TIME_WAIT状态，而client又发送的报文导致直接返回。
                1. 客户端（短连接但未主动关Socket）→ 发送带\n的登录请求；
                2. 服务端readFromClient识别\n → 返回完整请求，run()处理完登录逻辑；
                3. run()继续循环读取数据 → 客户端无新数据发送，readFromClient读到bytesRead=0（客户端未关但无数据）；
                4. run()循环退出 → 执行完毕 → Poco框架自动关闭Socket（服务端成为「主动关闭方」）；
                5. 服务端该连接进入TIME_WAIT状态（TCP主动关闭的必然结果）；
                6. 客户端此时仍持有旧Socket → 感知到连接被服务端关闭 → 报“服务器断开连接”（第一次登录失败）；
                7. 第二次点击登录 → 客户端新建Socket连接（用新的客户端端口，避开旧连接的TIME_WAIT）→ 服务端正常接收、处理 → 密码数据正常上传。

