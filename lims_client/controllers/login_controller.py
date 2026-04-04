# controllers/login_controller.py
from loguru import logger
from common.tcp_client import tcp_client
from models.user import User ,AESCrypto,PasswordCrypto, Argon2Crypto
from views.login_view import LoginView
from views.main_window import MainWindow
from controllers.experiment_controller import ExperimentController

# AES密钥（建议生产环境从配置文件读取，权限600）
AES_KEY = b"0123456789ABCDEF0123456789ABCDEF"  # 32字节（256位）

class LoginController:
    """登录业务控制器"""
    def __init__(self, login_view: LoginView):
        self.login_view = login_view    # 绑定了view中的类
        self.aes = AESCrypto(AES_KEY)
        self.argon2 = Argon2Crypto()

        self.login_view.login_signal.connect(self.handle_login) # 绑定界面信号，连接视图的信号到控制器的处理方法

    def handle_login(self, username: str, password: str):
        """处理登录逻辑"""
        try:

            """用户登录：AES加密明文密码，传给服务端验证"""
            # 1. AES加密明文密码（核心：传输加密后的明文，而非哈希）
            # cipher_hex, iv_hex, tag_hex = self.aes.encrypt(f"LOGIN|{username}|{password}")
            # arg2_pwd = f"{cipher_hex}|{iv_hex}|{tag_hex}"
            arg2_pwd = password     # 先使用明文实现功能


            # 关键：客户端先加密密码（和服务端盐值一致）  先注释掉
            # encrypted_pwd = User.encrypt_password(password, salt="lims@2025")

            # 2. TCP发送 发送登录请求到服务器
            response = tcp_client.send_request(
                cmd="user_login",
                data={"username" : username,
                      "password" : arg2_pwd    # 加密传输
                }
            )
            print(response)
            # 处理响应
            if response.get("code") == 200:
                # 解析用户信息
                # 登录成功，获取 token
                token = response.get("data", {}).get("token")
                if not token:
                    raise ValueError("登录成功，但未返回 token")

                # 创建用户对象并保存 token
                # user = User(username=username, token=token)
                user_data = response.get("data", {})
                user = User(
                    username = username,
                    token = user_data.get("token"),
                    role = username # user_data.get("role")
                )

                # logger.info(f"用户 {username} 登录成功")
                logger.info(f"用户 {username} (角色: {user.role}) 登录成功")
                # 关闭登录窗口，打开主窗口
                self.login_view.close()
                self.main_window = MainWindow(user)

                # 为主窗口中的视图创建并绑定控制器
                self.experiment_controller = ExperimentController(self.main_window.experiment_view)

                self.main_window.show()
            else:
                self.login_view.show_error(f"登录失败：{response.get('msg', '未知错误')}")

        except ConnectionError as e:
            self.login_view.show_error(f"网络错误：{str(e)}")
        except Exception as e:
            logger.error(f"登录异常：{e}")
            self.login_view.show_error(f"系统异常：{str(e)}")

    def handle_register(self, username: str, password: str):
        """用户注册：生成Argon2id哈希，传给服务端存储"""
        # 1. 生成Argon2id哈希
        argon2_hash = self.argon2.hash_password(password)
        # 2. 构造注册数据（AES加密哈希，避免传输泄露）
        cipher_hex, iv_hex, tag_hex = self.aes.encrypt(f"REGISTER|{username}|{argon2_hash}")
        arg2_pwd = f"{cipher_hex}|{iv_hex}|{tag_hex}"
        # 3. TCP发送
        response = tcp_client.send_request(
            cmd="user_login",
            data={"username": username,
                  "password": arg2_pwd  # 加密传输
                  }
        )
        print(f"注册响应：{response}")