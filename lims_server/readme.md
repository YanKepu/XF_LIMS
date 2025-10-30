# server模块开发


## 逻辑分层
| 层 |	核心职责  |	技术依赖 |
| ---| -- | -- |
|通信层（Network）|	处理 TCP 连接、解析 JSON 消息、分发请求 |	POCO Network（TCP 服务器）、nlohmann/json（JSON 解析）|
|业务逻辑层（Service）|	实现核心业务（登录、样品管理、任务分配等） |	无第三方依赖，纯 C++ 逻辑 |
|数据访问层（DAO）|	封装数据库操作（PostgreSQL）	|libpqxx（PostgreSQL C++ 客户端）|
|公共模块（Common） |	工具类（日志、配置、JWT 认证等）|	POCO Util（配置）、libjwt（令牌生成）|

## 代码架构
lab_server/
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