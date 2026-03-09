# 1. 软件架构

本文档定义了 LIMS 项目的软件架构，包括前端、后端和数据库之间的关系。

## 1.1. 总体架构

采用经典的客户端/服务器 (C/S) 架构。

- **`lims_server` (服务端)**:
  - 使用 C++11 开发。
  - 负责核心业务逻辑、数据处理和数据库交互。
  - 通过 RESTful API (基于 HTTP) 与客户端通信。
- **`lims_client` (客户端)**:
  - 使用 Python 和 PySide6 开发。
  - 提供图形用户界面 (GUI)。
  - 调用后端的 API 来完成用户操作。
- **数据库**:
  - 使用 PostgreSQL。
  - 存储所有持久化数据，如用户信息、实验数据等。

## 1.2. API 接口约定 (待细化)

我们将使用 JSON 作为数据交换格式。所有 API 都应有统一的前缀，例如 `/api/v1/`。

- **认证**: 使用 JWT (JSON Web Token) 进行无状态认证。
  - `POST /api/v1/auth/login`: 用户登录，成功后返回 JWT。
  - `POST /api/v1/auth/register`: 用户注册。
- **实验管理**:
  - `GET /api/v1/experiments`: 获取实验列表。
  - `POST /api/v1/experiments`: 创建新实验。
  - `GET /api/v1/experiments/{id}`: 获取特定实验的详情。

## 1.3. C++ 服务端架构 (待细化)

- **网络层**: 使用 Poco 的 `HTTPServer` 来处理 HTTP 请求。
- **路由层**: 将不同的 URL 路径分发到对应的 `HTTPRequestHandler`。
- **服务层**: 实现核心业务逻辑 (例如，`UserService`, `ExperimentService`)。
- **数据访问层**: 使用 `libpqxx` 与 PostgreSQL 数据库进行交互。
