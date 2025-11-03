-- 创建用户表
CREATE TABLE users (
    id SERIAL PRIMARY KEY,
    username VARCHAR(50) UNIQUE NOT NULL,
    password VARCHAR(100) NOT NULL,  -- 存储加密后的密码
    role VARCHAR(20) NOT NULL,       -- admin/operator
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- 创建样品表
CREATE TABLE samples (
    id SERIAL PRIMARY KEY,
    sample_code VARCHAR(50) UNIQUE NOT NULL,
    name VARCHAR(100) NOT NULL,
    status VARCHAR(20) NOT NULL,     -- pending/processing/completed
    created_by INT REFERENCES users(id),
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);