-- 创建数据库和用户
CREATE DATABASE lims_db;
CREATE USER lims_user WITH ENCRYPTED PASSWORD 'lims_pass';
GRANT ALL PRIVILEGES ON DATABASE lims_db TO lims_user;

-- 切换到目标数据库
\c lims_db;

-- 用户表（用于认证）
CREATE TABLE users (
    id SERIAL PRIMARY KEY,
    username VARCHAR(50) UNIQUE NOT NULL,
    password_hash VARCHAR(256) NOT NULL,
    role VARCHAR(20) NOT NULL,  -- admin/operator
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- 样品表
CREATE TABLE samples (
    id SERIAL PRIMARY KEY,
    sample_no VARCHAR(50) UNIQUE NOT NULL,
    name VARCHAR(100) NOT NULL,
    status VARCHAR(20) NOT NULL,  -- pending/processing/completed
    created_by INT REFERENCES users(id),
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- 任务表
CREATE TABLE tasks (
    id SERIAL PRIMARY KEY,
    task_name VARCHAR(100) NOT NULL,
    sample_id INT REFERENCES samples(id),
    assignee INT REFERENCES users(id),
    status VARCHAR(20) NOT NULL,  -- assigned/in_progress/completed
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);