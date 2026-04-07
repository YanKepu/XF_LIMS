# 标准通信格式和数据库设计

本文档定义了客户端与服务端之间的数据传输标准（JSON格式）以及数据在数据库中的存储格式。

## 实验管理模块

### 1. 客户端到服务端：创建实验请求 (JSON)

当客户端创建新实验时，将发送以下 JSON 格式的数据到服务端：

```json
{
  "cmd": "experiment",
  "data": {
    "action": "create_experiment",
    "experiment_name": "",      // 实验名称 (字符串，必填)
    "experiment_goal": "",      // 实验目标 (字符串，必填)
    "start_date": "",         // 开始日期 (字符串，格式: YYYY-MM-DD，必填)
    "end_date": "",           // 结束日期 (字符串，格式: YYYY-MM-DD，必填)
    "description": "",        // 实验描述 (字符串，可选)
    "status": ""              // 实验状态 (字符串，例如: "草稿", "进行中", "已完成", "暂停"，必填)
  }
  "timestamp": "2026-03-01 10:00:00"  // 请求时间戳 (字符串，格式: YYYY-MM-DD HH:MM:SS)
}
```

### 2. 服务端到客户端：创建实验响应 (JSON)

服务端在处理完创建实验请求后，将返回以下 JSON 格式的数据到客户端：

```json
{
  "status": "success",        // 请求状态 (字符串，"success" 或 "failure")
  "message": "",              // 状态消息 (字符串，例如: "实验创建成功", "创建失败: 实验名称已存在")
  "data": {
    "experiment_id": "",      // 新创建实验的唯一ID (字符串，成功时返回)
    "experiment_name": ""
  }
}
```

### 3. 数据库存储格式：实验表 (Experiment Table)

实验数据将在数据库中以如下结构存储：

```sql
TABLE experiments (
    id VARCHAR(36) PRIMARY KEY,      -- 实验的唯一标识符 (UUID)
    name VARCHAR(255) NOT NULL,      -- 实验名称
    goal TEXT NOT NULL,              -- 实验目标
    start_date DATE NOT NULL,        -- 开始日期
    end_date DATE NOT NULL,          -- 结束日期
    description TEXT,                -- 实验描述 (可为空)
    status VARCHAR(50) NOT NULL,     -- 实验状态
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP -- 创建时间
);
```

### 4. 客户端到服务端：获取实验列表请求 (JSON)

```json
{
  "action": "get_experiment_list",
  "data": {
    "search_term": "", 
    "page": 1,          
    "page_size": 10     
  }
}
```

### 5. 服务端到客户端：获取实验列表响应 (JSON)

```json
{
  "status": "success",
  "message": "",
  "data": {
    "total": 100,
    "experiments": [
      {
        "id": "uuid-1",
        "name": "实验A",
        "status": "进行中",
        "start_date": "2026-03-01",
        "end_date": "2026-04-01",
        "owner": "张三"
      }
    ]
  }
}
```

### 6. 客户端到服务端：更新实验请求 (JSON)

```json
{
  "action": "update_experiment",
  "data": {
    "id": "uuid-to-update", 
    "experiment_name": "更新后的实验名称",
    "experiment_goal": "更新后的实验目标"
  }
}
```

### 7. 客户端到服务端：删除实验请求 (JSON)

```json
{
  "action": "delete_experiment",
  "data": {
    "ids": ["uuid-1", "uuid-2"] 
  }
}
```

### 8. 通用成功/失败响应 (JSON)

对于更新、删除等没有复杂数据返回的操作，服务端将返回一个通用的响应。

```json
{
  "status": "success", 
  "message": "操作成功完成" 
}
```

## 实验管理模块

### 样本管理模块

### 设备管理模块

### 系统管理模块
