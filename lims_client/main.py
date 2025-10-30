import sys
from PySide6.QtWidgets import QApplication
from views.login_view import LoginView
from controllers.login_controller import LoginController
from models.tcp_client import TcpClient

if __name__ == "__main__":
    app = QApplication(sys.argv)

    # 初始化各层实例
    tcp_client = TcpClient()  # 模型（通信）
    login_view = LoginView()  # 视图（登录窗口）
    login_controller = LoginController(login_view, tcp_client)  # 控制器

    # 显示登录窗口
    login_view.show()

    sys.exit(app.exec())