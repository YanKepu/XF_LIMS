# views/system_admin_view.py
from PySide6.QtWidgets import QWidget, QLabel, QVBoxLayout
from PySide6.QtCore import Qt

class SystemAdminView(QWidget):
    """系统管理视图"""
    def __init__(self):
        super().__init__()
        layout = QVBoxLayout(self)
        label = QLabel("系统管理 (System Administration)")
        label.setAlignment(Qt.AlignCenter)
        layout.addWidget(label)
