# controllers/login_controller.py
from loguru import logger
from common.tcp_client import tcp_client
from models.user import User
from views.login_view import LoginView
from views.main_window import MainWindow

class LoginController:
    """登录业务控制器"""
    def __init__(self, login_view: LoginView):
        self.login_view = login_view    # 绑定了view中的类

        self.login_view.login_signal.connect(self.handle_login) # 绑定界面信号，连接视图的信号到控制器的处理方法

    def handle_login(self, username: str, password: str):
        """处理登录逻辑"""
        try:
            # 🔴 关键：客户端先加密密码（和服务端盐值一致）
            encrypted_pwd = User.encrypt_password(password, salt="lims@2025")

            # 发送登录请求到服务器
            response = tcp_client.send_request(
                cmd="user_login",
                data={"username" : username,
                      "password" : encrypted_pwd    # 加密传输
                }
            )

            # 处理响应
            if response.get("code") == 200:
                # 解析用户信息
                user = User.from_dict(response.get("data"))
                logger.info(f"用户 {username} 登录成功")
                # 关闭登录窗口，打开主窗口
                self.login_view.close()
                self.main_window = MainWindow(user)
                self.main_window.show()
            else:
                self.login_view.show_error(f"登录失败：{response.get('msg', '未知错误')}")

        except ConnectionError as e:
            self.login_view.show_error(f"网络错误：{str(e)}")
        except Exception as e:
            logger.error(f"登录异常：{e}")
            self.login_view.show_error(f"系统异常：{str(e)}")