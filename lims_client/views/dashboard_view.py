# views/dashboard_view.py
from PySide6.QtWidgets import QWidget, QLabel, QVBoxLayout
from PySide6.QtCore import Qt

class DashboardView(QWidget):
    """仪表盘视图"""
    def __init__(self):
        super().__init__()
        layout = QVBoxLayout(self)
        label = QLabel("仪表盘 (Dashboard)")
        label.setAlignment(Qt.AlignCenter)
        layout.addWidget(label)
