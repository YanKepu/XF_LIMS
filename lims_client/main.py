# main.py
import sys

from loguru import logger
from PySide6.QtWidgets import QApplication ,QMessageBox
from PySide6.QtCore import Qt, Signal, QMessageLogContext
from PySide6.QtGui import QIcon

from config.config import config
from common.tcp_client import tcp_client
from views.login_view import LoginView
from controllers.login_controller import LoginController

def setup_logger():
    """初始化日志"""
    logger.remove()  # 移除默认日志配置
    # 添加文件日志
    logger.add(
        config.LOG_PATH,
        level=config.LOG_LEVEL,
        rotation="100MB",
        retention="7 days",
        encoding="utf-8"
    )
    # 添加控制台日志
    logger.add(
        sys.stdout,
        level=config.LOG_LEVEL,
        colorize=True
    )

def main():
    """主函数"""
    # 初始化日志
    setup_logger()
    logger.info("LIMS客户端启动")

    # 创建Qt应用
    app = QApplication(sys.argv)
    app.setStyle("Fusion")  # 设置统一样式

    # 创建TCP client去连接服务端
    # logger.info("尝试连接LIMS服务器...")
    # connect_success = tcp_client.connect()
    # if not connect_success:
    #     # 连接失败弹窗提示
    #     msg = QMessageBox(
    #         QMessageBox.Warning,
    #         "连接失败",
    #         f"无法连接到服务器 {config.SERVER_HOST}:{config.SERVER_PORT}",
    #         QMessageBox.Ok,
    #         None,
    #         flags=Qt.WindowStaysOnTopHint  # 弹窗置顶
    #     )
    #     msg.setInformativeText("请检查：\n1. 服务器地址/端口是否正确\n2. 服务器是否已启动\n3. 网络是否通畅")
    #     msg.exec()

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
        logger.info("LIMS客户端退出")

    app.aboutToQuit.connect(exit_handler)

    # 运行应用
    sys.exit(app.exec())

if __name__ == "__main__":
    main()