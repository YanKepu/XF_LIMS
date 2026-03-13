# views/equipment_view.py
from PySide6.QtWidgets import QWidget, QLabel, QVBoxLayout
from PySide6.QtCore import Qt

class EquipmentView(QWidget):
    """设备管理视图"""
    def __init__(self):
        super().__init__()
        layout = QVBoxLayout(self)
        label = QLabel("设备管理 (Equipment Management)")
        label.setAlignment(Qt.AlignCenter)
        layout.addWidget(label)
