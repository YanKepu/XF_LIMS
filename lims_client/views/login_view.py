# views/login_view.py
from PySide6.QtWidgets import (
    QWidget, QLabel, QLineEdit, QPushButton,
    QVBoxLayout, QHBoxLayout, QMessageBox
)
from PySide6.QtCore import Qt, Signal
from PySide6.QtGui import QIcon

class LoginView(QWidget):
    """登录界面"""
    # 登录信号（用户名，密码）
    login_signal = Signal(str, str)

    def __init__(self):
        super().__init__()
        self.init_ui()

    def init_ui(self):
        # 窗口配置
        self.setWindowTitle("LIMS系统 - 登录")
        self.setFixedSize(400, 250)
        # self.setWindowIcon(QIcon("./static/icon.ico"))  # 可选：设置图标

        # 布局
        main_layout = QVBoxLayout()
        main_layout.setAlignment(Qt.AlignCenter)
        main_layout.setSpacing(20)

        # 标题
        title_label = QLabel("LIMS 客户端登录")
        title_label.setStyleSheet("font-size: 18px; font-weight: bold;")
        title_label.setAlignment(Qt.AlignCenter)
        main_layout.addWidget(title_label)

        # 用户名
        username_layout = QHBoxLayout()
        username_label = QLabel("用户名:")
        username_label.setFixedWidth(80)
        self.username_edit = QLineEdit()
        self.username_edit.setPlaceholderText("请输入用户名")
        username_layout.addWidget(username_label)
        username_layout.addWidget(self.username_edit)
        main_layout.addLayout(username_layout)

        # 密码
        password_layout = QHBoxLayout()
        password_label = QLabel("密  码:")
        password_label.setFixedWidth(80)
        self.password_edit = QLineEdit()
        self.password_edit.setPlaceholderText("请输入密码")
        self.password_edit.setEchoMode(QLineEdit.Password)
        password_layout.addWidget(password_label)
        password_layout.addWidget(self.password_edit)
        main_layout.addLayout(password_layout)

        # 登录按钮
        login_btn = QPushButton("登录")
        login_btn.setFixedWidth(100)
        login_btn.clicked.connect(self.on_login_click)
        btn_layout = QHBoxLayout()
        btn_layout.setAlignment(Qt.AlignCenter)
        btn_layout.addWidget(login_btn)
        main_layout.addLayout(btn_layout)

        self.setLayout(main_layout)

    def on_login_click(self):
        """登录按钮点击事件"""
        username = self.username_edit.text().strip()    # 获取用户名
        password = self.password_edit.text().strip()    # 获取密码
        if not username or not password:
            QMessageBox.warning(self, "提示", "用户名和密码不能为空！")
            return
        # 发送登录信号给控制器
        self.login_signal.emit(username, password)

    def show_error(self, msg: str):
        """显示错误提示"""
        QMessageBox.critical(self, "登录失败", msg)

    def clear_input(self):
        """清空输入框"""
        self.username_edit.clear()
        self.password_edit.clear()