
# 登录数据库
``` # 使用密码登录
psql -U lims_user -h localhost -p 5432 -d lims_db -W
```

查表
``` # 查看所有表
\dt

# 查看表结构

\d lims_experiment
```

创建表
``` # 创建新表
CREATE TABLE IF NOT EXISTS lims_experiment (
    experiment_id SERIAL PRIMARY KEY,
    experiment_name VARCHAR(255) NOT NULL,
    experiment_goal VARCHAR(255) NOT NULL,
    status VARCHAR(255) NOT NULL DEFAULT '草稿',
    timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
```


```
-- 插入测试实验数据
INSERT INTO experiments (id, name, goal, start_date, end_date, description, status) VALUES
('exp-001', '发动机性能测试', '测试新型发动机的性能参数', '2026-04-01', '2026-04-15', '测试发动机在不同工况下的性能表现', '进行中'),
('exp-002', '制动系统测试', '测试制动系统的可靠性', '2026-04-10', '2026-04-20', '测试制动系统在高温环境下的性能', '计划中'),
('exp-003', '电子控制系统测试', '测试ECU的稳定性', '2026-03-20', '2026-04-05', '测试电子控制系统在各种条件下的稳定性', '已完成');
```
-- 查看所有实验数据
SELECT * FROM experiments

-- 按状态查询实验数据
SELECT * FROM experiments WHERE status = '进行中'


\q # 退出数据库会