# views/experiment_view.py
from PySide6.QtWidgets import (
    QWidget, QLabel, QVBoxLayout, QHBoxLayout, QGroupBox,
    QFormLayout, QLineEdit, QPushButton, QTableWidget,
    QHeaderView, QAbstractItemView, QDateEdit, QTextEdit, QComboBox, QTableWidgetItem
)
from PySide6.QtCore import Qt, QDate

class ExperimentView(QWidget):
    """实验管理视图"""
    def __init__(self):
        super().__init__()
        main_layout = QVBoxLayout(self)

        # 1. 创建实验 (Create Experiment)
        create_experiment_group_box = QGroupBox("创建实验")
        create_experiment_layout = QFormLayout(create_experiment_group_box)

        self.experiment_name_input = QLineEdit()
        create_experiment_layout.addRow("实验名称:", self.experiment_name_input)

        self.experiment_goal_input = QLineEdit()
        create_experiment_layout.addRow("实验目标:", self.experiment_goal_input)

        self.start_date_edit = QDateEdit(QDate.currentDate())
        self.start_date_edit.setCalendarPopup(True)
        create_experiment_layout.addRow("开始日期:", self.start_date_edit)

        self.end_date_edit = QDateEdit(QDate.currentDate().addDays(7))
        self.end_date_edit.setCalendarPopup(True)
        create_experiment_layout.addRow("结束日期:", self.end_date_edit)

        self.experiment_description_input = QTextEdit()
        self.experiment_description_input.setPlaceholderText("请输入实验描述...")
        create_experiment_layout.addRow("实验描述:", self.experiment_description_input)

        self.experiment_status_combo = QComboBox()
        self.experiment_status_combo.addItems(["草稿", "进行中", "已完成", "暂停"])
        create_experiment_layout.addRow("状态:", self.experiment_status_combo)

        self.create_experiment_button = QPushButton("创建实验")
        create_experiment_layout.addRow(self.create_experiment_button)

        main_layout.addWidget(create_experiment_group_box)

        # 2. 实验列表 (Experiment List)
        experiment_list_group_box = QGroupBox("实验列表")
        experiment_list_layout = QVBoxLayout(experiment_list_group_box)

        search_layout = QHBoxLayout()
        self.search_input = QLineEdit()
        self.search_input.setPlaceholderText("搜索实验名称或负责人...")
        search_layout.addWidget(self.search_input)

        self.search_button = QPushButton("搜索")
        search_layout.addWidget(self.search_button)
        experiment_list_layout.addLayout(search_layout)

        self.experiment_table = QTableWidget()
        self.experiment_table.setColumnCount(5)
        self.experiment_table.setHorizontalHeaderLabels(["实验名称", "状态", "开始日期", "结束日期", "负责人"])
        self.experiment_table.horizontalHeader().setSectionResizeMode(QHeaderView.Stretch)
        self.experiment_table.setEditTriggers(QAbstractItemView.NoEditTriggers)
        self.experiment_table.setSelectionBehavior(QAbstractItemView.SelectRows)

        # Placeholder data
        self.experiment_table.insertRow(0)
        self.experiment_table.setItem(0, 0, QTableWidgetItem("项目A - 病毒分析"))
        self.experiment_table.setItem(0, 1, QTableWidgetItem("进行中"))
        self.experiment_table.setItem(0, 2, QTableWidgetItem("2026-03-01"))
        self.experiment_table.setItem(0, 3, QTableWidgetItem("2026-04-01"))
        self.experiment_table.setItem(0, 4, QTableWidgetItem("张三"))

        self.experiment_table.insertRow(1)
        self.experiment_table.setItem(1, 0, QTableWidgetItem("项目B - 细胞培养"))
        self.experiment_table.setItem(1, 1, QTableWidgetItem("已完成"))
        self.experiment_table.setItem(1, 2, QTableWidgetItem("2026-02-10"))
        self.experiment_table.setItem(1, 3, QTableWidgetItem("2026-02-28"))
        self.experiment_table.setItem(1, 4, QTableWidgetItem("李四"))


        experiment_list_layout.addWidget(self.experiment_table)
        main_layout.addWidget(experiment_list_group_box)

        main_layout.addStretch(1)
