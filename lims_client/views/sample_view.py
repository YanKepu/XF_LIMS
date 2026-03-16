# views/sample_view.py
from PySide6.QtWidgets import (
    QWidget, QLabel, QVBoxLayout, QHBoxLayout, QGroupBox,
    QFormLayout, QLineEdit, QPushButton, QTableWidget,
    QHeaderView, QAbstractItemView, QDateEdit, QTextEdit, QComboBox, QTableWidgetItem, QStackedWidget
)
from PySide6.QtCore import Qt, QDate

class SampleView(QWidget):
    """样本管理视图"""
    def __init__(self):
        super().__init__()
        self.main_layout = QVBoxLayout(self)
        self.stacked_widget = QStackedWidget()
        self.main_layout.addWidget(self.stacked_widget)

        self._setup_sample_list_view()
        self._setup_sample_registration_view()

        # Set initial view to sample list
        self.stacked_widget.setCurrentWidget(self.sample_list_container)

    def _setup_sample_list_view(self):
        self.sample_list_container = QWidget()
        container_layout = QVBoxLayout(self.sample_list_container)

        # 2. 样本列表 (Sample List)
        sample_list_group_box = QGroupBox("样本列表")
        sample_list_layout = QVBoxLayout(sample_list_group_box)

        header_layout = QHBoxLayout()
        self.search_sample_input = QLineEdit()
        self.search_sample_input.setPlaceholderText("搜索样本ID、类型或来源...")
        header_layout.addWidget(self.search_sample_input)

        self.search_sample_button = QPushButton("搜索")
        header_layout.addWidget(self.search_sample_button)

        self.register_new_sample_button = QPushButton("登记新样本")
        self.register_new_sample_button.clicked.connect(self._show_sample_registration_view)
        header_layout.addWidget(self.register_new_sample_button)

        sample_list_layout.addLayout(header_layout)

        self.sample_table = QTableWidget()
        self.sample_table.setColumnCount(6)
        self.sample_table.setHorizontalHeaderLabels(["样本ID", "类型", "来源", "数量", "存储位置", "状态"])
        self.sample_table.horizontalHeader().setSectionResizeMode(QHeaderView.Stretch)
        self.sample_table.setEditTriggers(QAbstractItemView.NoEditTriggers)
        self.sample_table.setSelectionBehavior(QAbstractItemView.SelectRows)

        # Placeholder data
        self.sample_table.insertRow(0)
        self.sample_table.setItem(0, 0, QTableWidgetItem("S001"))
        self.sample_table.setItem(0, 1, QTableWidgetItem("血液"))
        self.sample_table.setItem(0, 2, QTableWidgetItem("患者A"))
        self.sample_table.setItem(0, 3, QTableWidgetItem("5ml"))
        self.sample_table.setItem(0, 4, QTableWidgetItem("冰箱1, 架2"))
        self.sample_table.setItem(0, 5, QTableWidgetItem("可用"))

        self.sample_table.insertRow(1)
        self.sample_table.setItem(1, 0, QTableWidgetItem("S002"))
        self.sample_table.setItem(1, 1, QTableWidgetItem("组织"))
        self.sample_table.setItem(1, 2, QTableWidgetItem("小鼠B"))
        self.sample_table.setItem(1, 3, QTableWidgetItem("1g"))
        self.sample_table.setItem(1, 4, QTableWidgetItem("液氮罐3"))
        self.sample_table.setItem(1, 5, QTableWidgetItem("已使用"))

        sample_list_layout.addWidget(self.sample_table)
        container_layout.addWidget(sample_list_group_box)
        self.stacked_widget.addWidget(self.sample_list_container)

    def _setup_sample_registration_view(self):
        self.sample_registration_container = QWidget()
        container_layout = QVBoxLayout(self.sample_registration_container)

        # 1. 样本登记 (Sample Registration)
        sample_registration_group_box = QGroupBox("样本登记")
        sample_registration_layout = QFormLayout(sample_registration_group_box)

        self.sample_id_input = QLineEdit()
        sample_registration_layout.addRow("样本ID:", self.sample_id_input)

        self.sample_type_combo = QComboBox()
        self.sample_type_combo.addItems(["血液", "尿液", "组织", "DNA", "RNA"])
        sample_registration_layout.addRow("样本类型:", self.sample_type_combo)

        self.sample_source_input = QLineEdit()
        sample_registration_layout.addRow("来源:", self.sample_source_input)

        self.sample_quantity_input = QLineEdit() # Could be QDoubleSpinBox
        sample_registration_layout.addRow("数量:", self.sample_quantity_input)

        self.storage_location_input = QLineEdit()
        sample_registration_layout.addRow("存储位置:", self.storage_location_input)

        self.collection_date_edit = QDateEdit(QDate.currentDate())
        self.collection_date_edit.setCalendarPopup(True)
        sample_registration_layout.addRow("采集日期:", self.collection_date_edit)

        self.associated_experiment_input = QLineEdit()
        sample_registration_layout.addRow("关联实验:", self.associated_experiment_input)

        self.sample_remarks_input = QTextEdit()
        self.sample_remarks_input.setPlaceholderText("请输入备注...")
        sample_registration_layout.addRow("备注:", self.sample_remarks_input)

        action_buttons_layout = QHBoxLayout()
        self.register_sample_button = QPushButton("登记样本")
        self.back_to_list_button_sample = QPushButton("返回列表")
        self.back_to_list_button_sample.clicked.connect(self._show_sample_list_view)
        action_buttons_layout.addWidget(self.register_sample_button)
        action_buttons_layout.addWidget(self.back_to_list_button_sample)
        sample_registration_layout.addRow(action_buttons_layout)

        container_layout.addWidget(sample_registration_group_box)
        container_layout.addStretch(1)
        self.stacked_widget.addWidget(self.sample_registration_container)

    def _show_sample_registration_view(self):
        self.stacked_widget.setCurrentWidget(self.sample_registration_container)

    def _show_sample_list_view(self):
        self.stacked_widget.setCurrentWidget(self.sample_list_container)