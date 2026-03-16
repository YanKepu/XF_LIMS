# views/equipment_view.py
from PySide6.QtWidgets import (
    QWidget, QLabel, QVBoxLayout, QHBoxLayout, QGroupBox,
    QFormLayout, QLineEdit, QPushButton, QTableWidget,
    QHeaderView, QAbstractItemView, QDateEdit, QTextEdit, QComboBox, QTableWidgetItem
)
from PySide6.QtCore import Qt, QDate

class EquipmentView(QWidget):
    """设备管理视图"""
    def __init__(self):
        super().__init__()
        main_layout = QVBoxLayout(self)

        # 1. 设备列表 (Equipment List)
        equipment_list_group_box = QGroupBox("设备列表")
        equipment_list_layout = QVBoxLayout(equipment_list_group_box)

        search_layout = QHBoxLayout()
        self.search_equipment_input = QLineEdit()
        self.search_equipment_input.setPlaceholderText("搜索设备名称、型号或负责人...")
        search_layout.addWidget(self.search_equipment_input)

        self.search_equipment_button = QPushButton("搜索")
        search_layout.addWidget(self.search_equipment_button)
        equipment_list_layout.addLayout(search_layout)

        self.equipment_table = QTableWidget()
        self.equipment_table.setColumnCount(7)
        self.equipment_table.setHorizontalHeaderLabels(["设备名称", "型号", "制造商", "状态", "上次维护日期", "下次校准日期", "负责人"])
        self.equipment_table.horizontalHeader().setSectionResizeMode(QHeaderView.Stretch)
        self.equipment_table.setEditTriggers(QAbstractItemView.NoEditTriggers)
        self.equipment_table.setSelectionBehavior(QAbstractItemView.SelectRows)

        # Placeholder data
        self.equipment_table.insertRow(0)
        self.equipment_table.setItem(0, 0, QTableWidgetItem("离心机型号A"))
        self.equipment_table.setItem(0, 1, QTableWidgetItem("Centrifuge-A"))
        self.equipment_table.setItem(0, 2, QTableWidgetItem("厂商X"))
        self.equipment_table.setItem(0, 3, QTableWidgetItem("正常"))
        self.equipment_table.setItem(0, 4, QTableWidgetItem("2026-03-01"))
        self.equipment_table.setItem(0, 5, QTableWidgetItem("2026-09-01"))
        self.equipment_table.setItem(0, 6, QTableWidgetItem("王五"))

        self.equipment_table.insertRow(1)
        self.equipment_table.setItem(1, 0, QTableWidgetItem("光谱仪型号B"))
        self.equipment_table.setItem(1, 1, QTableWidgetItem("Spectrometer-B"))
        self.equipment_table.setItem(1, 2, QTableWidgetItem("厂商Y"))
        self.equipment_table.setItem(1, 3, QTableWidgetItem("维护中"))
        self.equipment_table.setItem(1, 4, QTableWidgetItem("2026-03-10"))
        self.equipment_table.setItem(1, 5, QTableWidgetItem("2026-06-10"))
        self.equipment_table.setItem(1, 6, QTableWidgetItem("赵六"))

        equipment_list_layout.addWidget(self.equipment_table)
        main_layout.addWidget(equipment_list_group_box)

        # 2. 维护记录 (Maintenance Records)
        maintenance_group_box = QGroupBox("维护记录")
        maintenance_layout = QVBoxLayout(maintenance_group_box)

        self.maintenance_table = QTableWidget()
        self.maintenance_table.setColumnCount(4)
        self.maintenance_table.setHorizontalHeaderLabels(["维护日期", "维护类型", "执行人", "描述"])
        self.maintenance_table.horizontalHeader().setSectionResizeMode(QHeaderView.Stretch)
        self.maintenance_table.setEditTriggers(QAbstractItemView.NoEditTriggers)
        self.maintenance_table.setSelectionBehavior(QAbstractItemView.SelectRows)

        # Placeholder data
        self.maintenance_table.insertRow(0)
        self.maintenance_table.setItem(0, 0, QTableWidgetItem("2026-03-01"))
        self.maintenance_table.setItem(0, 1, QTableWidgetItem("日常保养"))
        self.maintenance_table.setItem(0, 2, QTableWidgetItem("王五"))
        self.maintenance_table.setItem(0, 3, QTableWidgetItem("清洁，润滑"))

        self.add_maintenance_button = QPushButton("添加维护记录")
        maintenance_layout.addWidget(self.maintenance_table)
        maintenance_layout.addWidget(self.add_maintenance_button)
        main_layout.addWidget(maintenance_group_box)

        # 3. 校准计划 (Calibration Plan)
        calibration_group_box = QGroupBox("校准计划")
        calibration_layout = QVBoxLayout(calibration_group_box)

        self.calibration_table = QTableWidget()
        self.calibration_table.setColumnCount(4)
        self.calibration_table.setHorizontalHeaderLabels(["校准日期", "校准周期", "下次校准日期", "校准结果"])
        self.calibration_table.horizontalHeader().setSectionResizeMode(QHeaderView.Stretch)
        self.calibration_table.setEditTriggers(QAbstractItemView.NoEditTriggers)
        self.calibration_table.setSelectionBehavior(QAbstractItemView.SelectRows)

        # Placeholder data
        self.calibration_table.insertRow(0)
        self.calibration_table.setItem(0, 0, QTableWidgetItem("2026-03-01"))
        self.calibration_table.setItem(0, 1, QTableWidgetItem("6个月"))
        self.calibration_table.setItem(0, 2, QTableWidgetItem("2026-09-01"))
        self.calibration_table.setItem(0, 3, QTableWidgetItem("合格"))

        self.add_calibration_button = QPushButton("添加校准计划")
        calibration_layout.addWidget(self.calibration_table)
        calibration_layout.addWidget(self.add_calibration_button)
        main_layout.addWidget(calibration_group_box)

        main_layout.addStretch(1)
