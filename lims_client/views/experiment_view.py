# views/experiment_view.py
from PySide6.QtWidgets import (
    QWidget, QLabel, QVBoxLayout, QHBoxLayout, QGroupBox,
    QFormLayout, QLineEdit, QPushButton, QTableWidget,
    QHeaderView, QAbstractItemView, QDateEdit, QTextEdit, QComboBox, QTableWidgetItem, QStackedWidget, QMessageBox
)
from PySide6.QtCore import Qt, QDate

from common.tcp_client import tcp_client # Import tcp_client

class ExperimentView(QWidget):
    """实验管理视图"""
    def __init__(self):
        super().__init__()
        self.main_layout = QVBoxLayout(self)
        self.stacked_widget = QStackedWidget()
        self.main_layout.addWidget(self.stacked_widget)

        self._setup_experiment_list_view()
        self._setup_create_experiment_view()

        # Set initial view to experiment list
        self.stacked_widget.setCurrentWidget(self.experiment_list_container)

    def _setup_experiment_list_view(self):
        self.experiment_list_container = QWidget()
        container_layout = QVBoxLayout(self.experiment_list_container)

        # 2. 实验列表 (Experiment List)
        experiment_list_group_box = QGroupBox("实验列表")
        experiment_list_layout = QVBoxLayout(experiment_list_group_box)

        header_layout = QHBoxLayout()
        self.search_input = QLineEdit()
        self.search_input.setPlaceholderText("搜索实验名称或负责人...")
        header_layout.addWidget(self.search_input)

        self.search_button = QPushButton("搜索")
        header_layout.addWidget(self.search_button)

        self.create_new_experiment_button = QPushButton("创建新实验")
        self.create_new_experiment_button.clicked.connect(self._show_create_experiment_view)
        header_layout.addWidget(self.create_new_experiment_button)

        experiment_list_layout.addLayout(header_layout)

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
        container_layout.addWidget(experiment_list_group_box)
        self.stacked_widget.addWidget(self.experiment_list_container)

    def _setup_create_experiment_view(self):
        self.create_experiment_container = QWidget()
        container_layout = QVBoxLayout(self.create_experiment_container)

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

        action_buttons_layout = QHBoxLayout()
        self.create_experiment_button = QPushButton("创建实验")
        self.create_experiment_button.clicked.connect(self._create_experiment) # Connect to new method
        self.back_to_list_button_exp = QPushButton("返回列表")
        self.back_to_list_button_exp.clicked.connect(self._show_experiment_list_view)
        action_buttons_layout.addWidget(self.create_experiment_button)
        action_buttons_layout.addWidget(self.back_to_list_button_exp)
        create_experiment_layout.addRow(action_buttons_layout)

        container_layout.addWidget(create_experiment_group_box)
        container_layout.addStretch(1)
        self.stacked_widget.addWidget(self.create_experiment_container)

    def _create_experiment(self):
        # Collect data from form fields
        experiment_name = self.experiment_name_input.text()
        experiment_goal = self.experiment_goal_input.text()
        start_date = self.start_date_edit.date().toString("yyyy-MM-dd")
        end_date = self.end_date_edit.date().toString("yyyy-MM-dd")
        description = self.experiment_description_input.toPlainText()
        status = self.experiment_status_combo.currentText()

        # Basic validation
        if not all([experiment_name, experiment_goal, start_date, end_date, status]):
            QMessageBox.warning(self, "输入错误", "请填写所有必填字段：实验名称、实验目标、开始日期、结束日期、状态。")
            return

        # Construct JSON payload
        import json
        payload = {
            "action": "create_experiment",
            "data": {
                "experiment_name": experiment_name,
                "experiment_goal": experiment_goal,
                "start_date": start_date,
                "end_date": end_date,
                "description": description,
                "status": status
            }
        }

        # Send message to server
        response = tcp_client.send_message(json.dumps(payload))
        if response:
            try:
                response_data = json.loads(response)
                if response_data.get("status") == "success":
                    QMessageBox.information(self, "成功", response_data.get("message", "实验创建成功！"))
                    # Clear form fields
                    self.experiment_name_input.clear()
                    self.experiment_goal_input.clear()
                    self.experiment_description_input.clear()
                    self.start_date_edit.setDate(QDate.currentDate())
                    self.end_date_edit.setDate(QDate.currentDate().addDays(7))
                    self.experiment_status_combo.setCurrentIndex(0)
                    self.back_to_list_button_exp.click() # Go back to list view
                else:
                    QMessageBox.warning(self, "失败", response_data.get("message", "实验创建失败！"))
            except json.JSONDecodeError:
                QMessageBox.critical(self, "错误", "服务器响应格式错误。")
        else:
            QMessageBox.critical(self, "错误", "未能收到服务器响应或连接失败。")


    def _show_create_experiment_view(self):
        self.stacked_widget.setCurrentWidget(self.create_experiment_container)

    def _show_experiment_list_view(self):
        self.stacked_widget.setCurrentWidget(self.experiment_list_container)