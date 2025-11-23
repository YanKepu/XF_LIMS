# config/config.py
import os
from dotenv import load_dotenv

# 加载.env配置文件（可选，方便环境切换）
load_dotenv()

class Config:
    """系统配置类"""
    # TCP服务器配置
    SERVER_HOST = os.getenv("LIMS_SERVER_HOST", "152.136.97.238")
    SERVER_PORT = int(os.getenv("LIMS_SERVER_PORT", 8080))
    # 通信超时时间（秒）
    TCP_TIMEOUT = 10
    # 日志配置
    LOG_LEVEL = os.getenv("LOG_LEVEL", "INFO")
    LOG_PATH = os.getenv("LOG_PATH", "./lims_client.log")

# 实例化配置对象
config = Config()