# models/user.py
from dataclasses import dataclass
from dataclasses_json import dataclass_json
from typing import Optional

@dataclass_json
@dataclass
class User:
    """用户数据模型"""
    username: str          # 用户名
    password: Optional[str] = None  # 密码（传输时加密）
    token: Optional[str] = None     # 登录令牌
    role: Optional[str] = None      # 用户角色（管理员/操作员）
    id: Optional[int] = None        # 用户ID