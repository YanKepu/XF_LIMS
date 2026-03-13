# views/experiment_view.py
from PySide6.QtWidgets import QWidget, QLabel, QVBoxLayout
from PySide6.QtCore import Qt

class ExperimentView(QWidget):
    """实验管理视图"""
    def __init__(self):
        super().__init__()
        layout = QVBoxLayout(self)
        label = QLabel("实验管理 (Experiment Management)")
        label.setAlignment(Qt.AlignCenter)
        layout.addWidget(label)
