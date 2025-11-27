# models/user.py
from dataclasses import dataclass
from dataclasses_json import dataclass_json
from typing import Optional
# 基础导入（必选）
import hashlib
# 可选：生成安全的随机salt（推荐）
import secrets

@dataclass_json
@dataclass
class User:
    # """用户数据模型"""
    # username: str          # 用户名
    # password: Optional[str] = None  # 密码（传输时加密）
    # token: Optional[str] = None     # 登录令牌
    # role: Optional[str] = None      # 用户角色（管理员/操作员）
    # id: Optional[int] = None        # 用户ID

    @staticmethod
    def encrypt_password(password: str, salt: str, iterations: int = 100000) -> str:
        """
        安全的密码加密：PBKDF2_HMAC + SHA256 + 盐值
        :param password: 原始密码
        :param salt: 盐值
        :param iterations: 迭代次数（越大越安全，默认10万次）
        :return: 加密后的哈希字符串
        改进：后续可以使用随机盐值的方式，将用户名和盐值存入数据库
        """
        if not password or not salt:
            raise ValueError("密码和盐值不能为空")

        # 转 bytes
        password_bytes = password.encode("utf-8")
        salt_bytes = salt.encode("utf-8")

        # PBKDF2_HMAC 加密
        hash_obj = hashlib.pbkdf2_hmac(
            hash_name="sha256",  # 哈希算法
            password=password_bytes,
            salt=salt_bytes,
            iterations=iterations  # 迭代次数，建议10万+
        )

        # 转为16进制字符串
        encrypted_pwd = hash_obj.hex()
        return encrypted_pwd