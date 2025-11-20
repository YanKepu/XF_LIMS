# main.py
import sys
from loguru import logger
from PySide6.QtWidgets import QApplication
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

    # 初始化界面和控制器
    login_view = LoginView()
    login_controller = LoginController(login_view)
    login_view.show()

    # 程序退出处理
    def exit_handler():
        tcp_client.close()
        logger.info("LIMS客户端退出")

    app.aboutToQuit.connect(exit_handler)

    # 运行应用
    sys.exit(app.exec())

if __name__ == "__main__":
    main()