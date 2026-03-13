# views/main_window.py
from PySide6.QtWidgets import QMainWindow, QWidget, QHBoxLayout, QListWidget, QStackedWidget
from PySide6.QtCore import Qt
from models.user import User

class MainWindow(QMainWindow):
    """主界面"""
    def __init__(self, user: User):
        super().__init__()
        self.user = user
        self.init_ui()

    def init_ui(self):
        # 窗口配置
        self.setWindowTitle(f"LIMS系统 - {self.user.username}")
        self.resize(1024, 768)

        # 中心部件
        central_widget = QWidget()
        self.setCentralWidget(central_widget)

        # 主布局
        main_layout = QHBoxLayout(central_widget)
        main_layout.setContentsMargins(0, 0, 0, 0)
        main_layout.setSpacing(0)

        # 左侧导航栏
        self.nav_bar = QListWidget()
        self.nav_bar.setFixedWidth(150)
        main_layout.addWidget(self.nav_bar)

        # 右侧内容区
        self.stacked_widget = QStackedWidget()
        main_layout.addWidget(self.stacked_widget)
