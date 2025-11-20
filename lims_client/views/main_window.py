# views/main_window.py
from PySide6.QtWidgets import QMainWindow, QLabel, QWidget, QVBoxLayout
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
        self.setWindowTitle(f"LIMS系统 - 主界面（{self.user.username}）")
        self.resize(800, 600)

        # 中心部件
        central_widget = QWidget()
        self.setCentralWidget(central_widget)

        # 布局
        layout = QVBoxLayout()
        layout.setAlignment(Qt.AlignCenter)

        # 欢迎信息
        welcome_label = QLabel(f"欢迎使用LIMS系统！\n当前用户：{self.user.username}\n角色：{self.user.role}")
        welcome_label.setStyleSheet("font-size: 16px;")
        welcome_label.setAlignment(Qt.AlignCenter)
        layout.addWidget(welcome_label)

        central_widget.setLayout(layout)