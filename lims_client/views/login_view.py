from PySide6.QtWidgets import (QWidget, QVBoxLayout, QLabel, QLineEdit,
                               QPushButton, QMessageBox)
from PySide6.QtCore import Signal

class LoginView(QWidget):
    # 自定义信号：登录按钮点击（传递账号密码）
    login_clicked = Signal(str, str)

    def __init__(self):
        super().__init__()
        self.setWindowTitle("登录 - 实验室管理系统")
        self.setFixedSize(300, 200)
        self._init_ui()

    def _init_ui(self):
        """初始化界面组件"""
        layout = QVBoxLayout()

        # 账号输入
        self.lbl_username = QLabel("账号：")
        self.edit_username = QLineEdit()
        self.edit_username.setText("admin")  # 测试用

        # 密码输入
        self.lbl_password = QLabel("密码：")
        self.edit_password = QLineEdit()
        self.edit_password.setEchoMode(QLineEdit.Password)
        self.edit_password.setText("123456")  # 测试用

        # 登录按钮
        self.btn_login = QPushButton("登录")
        self.btn_login.clicked.connect(self._on_login_click)

        # 添加组件到布局
        layout.addWidget(self.lbl_username)
        layout.addWidget(self.edit_username)
        layout.addWidget(self.lbl_password)
        layout.addWidget(self.edit_password)
        layout.addWidget(self.btn_login)

        self.setLayout(layout)

    def _on_login_click(self):
        """登录按钮点击：发射信号给控制器"""
        username = self.edit_username.text().strip()
        password = self.edit_password.text().strip()
        if not username or not password:
            QMessageBox.warning(self, "输入错误", "账号和密码不能为空")
            return
        self.login_clicked.emit(username, password)  # 只发射信号，不处理逻辑

    def show_error(self, msg: str):
        """显示错误提示（由控制器调用）"""
        QMessageBox.warning(self, "错误", msg)

    def clear_input(self):
        """清空输入框（由控制器调用）"""
        self.edit_password.clear()