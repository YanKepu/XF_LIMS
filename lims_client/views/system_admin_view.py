# views/system_admin_view.py
from PySide6.QtWidgets import (
    QWidget, QLabel, QVBoxLayout, QHBoxLayout, QGroupBox,
    QFormLayout, QLineEdit, QPushButton, QTableWidget,
    QHeaderView, QAbstractItemView, QComboBox, QCheckBox, QTableWidgetItem
)
from PySide6.QtCore import Qt

class SystemAdminView(QWidget):
    """系统管理视图"""
    def __init__(self):
        super().__init__()
        main_layout = QVBoxLayout(self)

        # 1. 用户管理 (User Management)
        user_management_group_box = QGroupBox("用户管理")
        user_management_layout = QVBoxLayout(user_management_group_box)

        user_actions_layout = QHBoxLayout()
        self.add_user_button = QPushButton("添加用户")
        self.edit_user_button = QPushButton("编辑用户")
        self.reset_password_button = QPushButton("重置密码")
        self.delete_user_button = QPushButton("删除用户")
        user_actions_layout.addWidget(self.add_user_button)
        user_actions_layout.addWidget(self.edit_user_button)
        user_actions_layout.addWidget(self.reset_password_button)
        user_actions_layout.addWidget(self.delete_user_button)
        user_management_layout.addLayout(user_actions_layout)

        self.user_table = QTableWidget()
        self.user_table.setColumnCount(4)
        self.user_table.setHorizontalHeaderLabels(["用户名", "角色", "邮箱", "状态"])
        self.user_table.horizontalHeader().setSectionResizeMode(QHeaderView.Stretch)
        self.user_table.setEditTriggers(QAbstractItemView.NoEditTriggers)
        self.user_table.setSelectionBehavior(QAbstractItemView.SelectRows)

        # Placeholder data
        self.user_table.insertRow(0)
        self.user_table.setItem(0, 0, QTableWidgetItem("admin"))
        self.user_table.setItem(0, 1, QTableWidgetItem("管理员"))
        self.user_table.setItem(0, 2, QTableWidgetItem("admin@example.com"))
        self.user_table.setItem(0, 3, QTableWidgetItem("活跃"))

        self.user_table.insertRow(1)
        self.user_table.setItem(1, 0, QTableWidgetItem("user1"))
        self.user_table.setItem(1, 1, QTableWidgetItem("实验员"))
        self.user_table.setItem(1, 2, QTableWidgetItem("user1@example.com"))
        self.user_table.setItem(1, 3, QTableWidgetItem("活跃"))

        user_management_layout.addWidget(self.user_table)
        main_layout.addWidget(user_management_group_box)

        # 2. 权限管理 (Permission Management)
        permission_management_group_box = QGroupBox("权限管理")
        permission_management_layout = QVBoxLayout(permission_management_group_box)

        # For simplicity, using QTableWidget to show modules and roles, with checkboxes
        self.permission_table = QTableWidget()
        self.permission_table.setColumnCount(4) # Module, Admin, Experimenter, Guest
        self.permission_table.setHorizontalHeaderLabels(["模块/功能", "管理员", "实验员", "访客"])
        self.permission_table.horizontalHeader().setSectionResizeMode(QHeaderView.Stretch)
        self.permission_table.setEditTriggers(QAbstractItemView.NoEditTriggers) # Make cells read-only for module names

        # Placeholder data for permissions
        # Row for Dashboard
        self.permission_table.insertRow(0)
        self.permission_table.setItem(0, 0, QTableWidgetItem("仪表盘"))
        self._add_checkbox_to_table(self.permission_table, 0, 1, True) # Admin can access
        self._add_checkbox_to_table(self.permission_table, 0, 2, True) # Experimenter can access
        self._add_checkbox_to_table(self.permission_table, 0, 3, True) # Guest can access

        # Row for Experiment Management
        self.permission_table.insertRow(1)
        self.permission_table.setItem(1, 0, QTableWidgetItem("实验管理"))
        self._add_checkbox_to_table(self.permission_table, 1, 1, True)
        self._add_checkbox_to_table(self.permission_table, 1, 2, True)
        self._add_checkbox_to_table(self.permission_table, 1, 3, False)

        # Row for System Administration
        self.permission_table.insertRow(2)
        self.permission_table.setItem(2, 0, QTableWidgetItem("系统管理"))
        self._add_checkbox_to_table(self.permission_table, 2, 1, True)
        self._add_checkbox_to_table(self.permission_table, 2, 2, False)
        self._add_checkbox_to_table(self.permission_table, 2, 3, False)


        permission_management_layout.addWidget(self.permission_table)
        main_layout.addWidget(permission_management_group_box)

        main_layout.addStretch(1) # Add stretch to push content to top

    def _add_checkbox_to_table(self, table, row, column, checked):
        checkbox = QCheckBox()
        checkbox.setChecked(checked)
        checkbox.setStyleSheet("margin-left:auto; margin-right:auto;") # Center checkbox
        table.setCellWidget(row, column, checkbox)
