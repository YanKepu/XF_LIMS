# main.py
import sys

from PySide6.QtWidgets import QApplication ,QMessageBox
from PySide6.QtCore import Qt, Signal, QMessageLogContext
from PySide6.QtGui import QIcon

from config.config import config
from common.tcp_client import tcp_client
from views.login_view import LoginView
from controllers.login_controller import LoginController

def main():
    """主函数"""
    print("LIMS客户端启动")

    # 创建Qt应用
    app = QApplication(sys.argv)
    app.setStyle("Fusion")  # 设置统一样式

    # 创建TCP client去连接服务端
    print("尝试连接LIMS服务器...")
    connect_success = tcp_client.connect()
    if not connect_success:
        # 连接失败弹窗提示
        msg = QMessageBox(
            QMessageBox.Warning,
            "连接失败",
            f"无法连接到服务器 {config.SERVER_HOST}:{config.SERVER_PORT}",
            QMessageBox.Ok,
            None,
            flags=Qt.WindowStaysOnTopHint  # 弹窗置顶
        )
        msg.setInformativeText("请检查：\n1. 服务器地址/端口是否正确\n2. 服务器是否已启动\n3. 网络是否通畅")
        msg.exec()

    """
    初始化界面和控制器，使用了MVC（Model-View-Control）思想，将页面、控制、数据处理解耦
    用户操作 → View 感知操作 → 发信号给 Controller → Controller 调用 Model 处理数据 → Model 返回结果 → Controller 通知 View 更新展示
    """
    login_view = LoginView()        # 创建UI实例
    login_controller = LoginController(login_view)  # 创建控制器实例，将login_view实例保存
    login_view.show()       # 进行UI展示

    # 程序退出处理
    def exit_handler():
        tcp_client.close()
        print("LIMS客户端退出")

    app.aboutToQuit.connect(exit_handler)

    # 运行应用
    sys.exit(app.exec())

if __name__ == "__main__":
    main()