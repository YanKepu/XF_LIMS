# 技术栈

| 框架 | 版本             | 
|----|----------------|
| 语言 | python3.13     |
| GUI框架 | PySide6 6.10.0 |
| 网络通信 | PySide6 |
| JSON处理 | 内置json库 | 
|界面设计 | Qt Designer |



# 代码架构
```plaintext
lims_client/
├── common/                # 公共模块
│   ├── __init__.py
│   ├── tcp_client.py      # TCP通信核心封装
│   └── json_utils.py      # JSON序列化/反序列化工具
├── config/                # 配置模块
│   ├── __init__.py
│   └── config.py          # 系统配置（服务器地址、端口等）
├── controllers/           # 业务控制器
│   ├── __init__.py
│   └── login_controller.py# 登录业务逻辑
├── models/                # 数据模型
│   ├── __init__.py
│   └── user.py            # 用户数据模型
├── views/                 # 界面模块
│   ├── __init__.py
│   ├── login_view.py      # 登录界面
│   └── main_window.py     # 主界面
├── static/                # 静态资源（可选）
│   └── icon.ico           # 程序图标
├── main.py                # 程序入口
├── requirements.txt       # 依赖清单
└── main.spec              # PyInstaller打包配置
```


工作之后一直在做驱动，没有做过软件方面的。
1、密码需要加密传输
2、登录的界面符合MVC架构即模块 界面 控制分离解耦

json报文格式


| 字段      | 类型     | 必选 | 说明             | 
|---------|--------|----|----------------|
| version | string | 是  | 协议版本           |
| cmd     | string | 是  | 业务命令字（全小写_下划线） |
| data    | object | 是  | 业务专属数据结构       |
| seq     | string | 是  | 请求序列号 |
| timestamp | string | 是 | 请求时间戳 |
| token | string | 否 | 登录后的身份令牌 |

~~~
{
    "version":"v1.0"
        "cmd":"user_login",
        "data":
        {
            "username":"admin"
            "password":"dadgbfa"  //加密后的密码
        }
        “seq":"seqfdadf"   // 先不做
        "timestamp":"2025-11-25 16:30:45"
}
~~~





