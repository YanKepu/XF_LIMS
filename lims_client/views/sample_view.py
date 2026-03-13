# views/sample_view.py
from PySide6.QtWidgets import QWidget, QLabel, QVBoxLayout
from PySide6.QtCore import Qt

class SampleView(QWidget):
    """样本管理视图"""
    def __init__(self):
        super().__init__()
        layout = QVBoxLayout(self)
        label = QLabel("样本管理 (Sample Management)")
        label.setAlignment(Qt.AlignCenter)
        layout.addWidget(label)
