# views/dashboard_view.py
from PySide6.QtWidgets import QWidget, QLabel, QVBoxLayout, QGroupBox, QListWidget, QHBoxLayout, QPushButton
from PySide6.QtCore import Qt

class DashboardView(QWidget):
    """仪表盘视图"""
    def __init__(self):
        super().__init__()
        main_layout = QVBoxLayout(self)

        # 我的任务 GroupBox
        task_group_box = QGroupBox("我的任务")
        task_layout = QVBoxLayout(task_group_box)

        # 进行中任务
        in_progress_tasks_label = QLabel("进行中任务:")
        self.in_progress_tasks_list = QListWidget()
        self.in_progress_tasks_list.addItem("任务 A (截止: 2026-03-20)")
        self.in_progress_tasks_list.addItem("任务 B (截止: 2026-03-25)")
        task_layout.addWidget(in_progress_tasks_label)
        task_layout.addWidget(self.in_progress_tasks_list)

        # 已完成任务
        completed_tasks_label = QLabel("已完成任务:")
        self.completed_tasks_list = QListWidget()
        self.completed_tasks_list.addItem("任务 C (完成: 2026-03-15)")
        task_layout.addWidget(completed_tasks_label)
        task_layout.addWidget(self.completed_tasks_list)

        # 未开始任务
        not_started_tasks_label = QLabel("未开始任务:")
        self.not_started_tasks_list = QListWidget()
        self.not_started_tasks_list.addItem("任务 D (计划: 2026-03-28)")
        task_layout.addWidget(not_started_tasks_label)
        task_layout.addWidget(self.not_started_tasks_list)

        main_layout.addWidget(task_group_box)

        # 我的实验 GroupBox
        experiment_group_box = QGroupBox("我的实验")
        experiment_layout = QVBoxLayout(experiment_group_box)

        # 进行中实验
        in_progress_experiments_label = QLabel("进行中实验:")
        self.in_progress_experiments_list = QListWidget()
        self.in_progress_experiments_list.addItem("实验 X (阶段: 数据采集)")
        self.in_progress_experiments_list.addItem("实验 Y (阶段: 分析中)")
        experiment_layout.addWidget(in_progress_experiments_label)
        experiment_layout.addWidget(self.in_progress_experiments_list)

        # 已完成实验
        completed_experiments_label = QLabel("已完成实验:")
        self.completed_experiments_list = QListWidget()
        self.completed_experiments_list.addItem("实验 Z (完成: 2026-03-10)")
        experiment_layout.addWidget(completed_experiments_label)
        experiment_layout.addWidget(self.completed_experiments_list)

        # 未开始实验
        not_started_experiments_label = QLabel("未开始实验:")
        self.not_started_experiments_list = QListWidget()
        self.not_started_experiments_list.addItem("实验 W (计划: 2026-04-01)")
        experiment_layout.addWidget(not_started_experiments_label)
        experiment_layout.addWidget(self.not_started_experiments_list)

        main_layout.addWidget(experiment_group_box)

        main_layout.addStretch(1)
