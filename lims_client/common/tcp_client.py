# common/tcp_client.py
import socket
import datetime
from typing import Dict, Any
from config.config import config
from common.json_utils import json_serialize, json_deserialize

class LimsTCPClient:
    """LIMS TCP客户端封装"""
    def __init__(self):
        self.host = config.SERVER_HOST
        self.port = config.SERVER_PORT
        self.socket = None
        self.connected = False

    def connect(self) -> bool:
        """连接服务器"""
        try:
            self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.socket.settimeout(config.TCP_TIMEOUT)
            self.socket.connect((self.host, self.port))
            self.connected = True
            print(f"成功连接到服务器 {self.host}:{self.port}")
            return True
        except Exception as e:
            print(f"连接服务器失败: {e}")
            self.connected = False
            return False

    def send_request(self, cmd: str, data: Dict[str, Any]) -> Dict[str, Any]:
        """
        发送请求到服务器
        :param cmd: 命令字（如login、query_sample）
        :param data: 业务数据
        :return: 服务器响应数据
        """
        # 1. 每次请求都新建Socket（核心：不复用旧连接）
        self.close()  # 先关闭旧连接
        if not self.connect():
            raise ConnectionError("未连接到服务器")

        # 封装请求体
        request = {
            "cmd": cmd,
            "data": data
        }

        try:
            # 发送数据（UTF-8编码）
            send_data = json_serialize(request) + "\n"  # 换行符作为消息结束符
            self.socket.sendall(send_data.encode("utf-8"))
            print(f"发送请求: {send_data.strip()}")

            # 接收响应（按行读取）
            recv_data = b""
            while True:
                chunk = self.socket.recv(4096)
                if not chunk:
                    raise ConnectionAbortedError("服务器断开连接")
                recv_data += chunk
                if b"\n" in recv_data:
                    break

            # 解析响应
            response = json_deserialize(recv_data.decode("utf-8").strip())
            print(f"接收响应: {response}")
            return response

        except Exception as e:
            print(f"请求发送失败: {e}")
            self.connected = False
            raise

    def close(self):
        """关闭连接"""
        if self.socket:
            self.socket.close()
            self.connected = False
            print("已断开与服务器的连接")

# 单例模式（全局唯一TCP客户端）
tcp_client = LimsTCPClient()