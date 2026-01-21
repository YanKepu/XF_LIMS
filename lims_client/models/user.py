# models/user.py
import socket
import argon2
import argon2.exceptions
from argon2 import PasswordHasher
from dataclasses import dataclass
from dataclasses_json import dataclass_json
from Crypto.Cipher import AES
from Crypto.Random import get_random_bytes
from Crypto.Util.Padding import pad, unpad
import hashlib
import hmac
from typing import Optional
# 基础导入（必选）
import hashlib
# 可选：生成安全的随机salt（推荐）
import secrets

# -------------------------- 配置项 --------------------------
# AES密钥（建议生产环境从配置文件读取，权限600）
AES_KEY = b"0123456789ABCDEF0123456789ABCDEF"  # 32字节（256位）
# Argon2id参数（客户端/服务端必须完全一致）
ARGON2_TIME_COST = 3          # 时间成本（迭代次数）
ARGON2_MEMORY_COST = 65536    # 内存成本（64MB）
ARGON2_PARALLELISM = 4        # 并行度（CPU核心数）
ARGON2_HASH_LEN = 32          # 哈希长度
ARGON2_SALT_LEN = 16          # 盐值长度
# TCP配置
SERVER_IP = "127.0.0.1"
SERVER_PORT = 8888

# -------------------------- 1. 独立的密码工具层（通用，无业务耦合） --------------------------
class PasswordCrypto:
    """纯密码处理工具：仅负责生成/校验Argon2id哈希，无任何业务逻辑"""
    def __init__(self):
        self.ph = PasswordHasher(
            time_cost=ARGON2_TIME_COST,
            memory_cost=ARGON2_MEMORY_COST,
            parallelism=ARGON2_PARALLELISM,
            hash_len=ARGON2_HASH_LEN,
            salt_len=ARGON2_SALT_LEN,
            type=argon2.Type.ID
        )

    def generate_hash(self, plain_pwd: str) -> str:
        """生成Argon2id哈希（仅做哈希，无其他）"""
        return self.ph.hash(plain_pwd)

    def verify_hash(self, hash_str: str, plain_pwd: str) -> bool:
        """校验哈希（仅做验证，无其他）"""
        try:
            return self.ph.verify(hash_str, plain_pwd)
        except argon2.exceptions.VerifyMismatchError:
            return False

# -------------------------- 加密工具函数 --------------------------
class AESCrypto:
    """AES-GCM加密（防篡改+防重放）"""
    def __init__(self, key: bytes):
        self.key = key
        self.iv_size = 12  # GCM推荐12字节IV

    def encrypt(self, plain_text: str) -> tuple[str, str, str]:
        """加密明文，返回(密文hex, IV hex, 标签hex)"""
        plain_bytes = plain_text.encode("utf-8")
        iv = get_random_bytes(self.iv_size)  # 随机IV
        cipher = AES.new(self.key, AES.MODE_GCM, iv)
        ciphertext, tag = cipher.encrypt_and_digest(pad(plain_bytes, AES.block_size))
        # 转16进制便于传输
        cipher_hex = ciphertext.hex()
        iv_hex = iv.hex()
        tag_hex = tag.hex()
        return cipher_hex, iv_hex, tag_hex

# -------------------------- Argon2id工具函数 --------------------------
class Argon2Crypto:
    def __init__(self):
        self.ph = PasswordHasher(
            time_cost=ARGON2_TIME_COST,
            memory_cost=ARGON2_MEMORY_COST,
            parallelism=ARGON2_PARALLELISM,
            hash_len=ARGON2_HASH_LEN,
            salt_len=ARGON2_SALT_LEN,
            type=argon2.Type.ID  # 必须指定Argon2id
        )

    def hash_password(self, plain_pwd: str) -> str:
        """生成Argon2id哈希（自动生成盐值，哈希串包含所有参数）"""
        return self.ph.hash(plain_pwd)

    def verify_password(self, hash_str: str, plain_pwd: str) -> bool:
        """验证密码（常量时间对比）"""
        try:
            return self.ph.verify(hash_str, plain_pwd)
        except argon2.exceptions.VerifyMismatchError:
            return False



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