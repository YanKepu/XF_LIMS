# views/experiment_view.py
from PySide6.QtWidgets import QWidget, QVBoxLayout, QHBoxLayout, QPushButton, QTableView, QHeaderView

class ExperimentView(QWidget):
    """实验管理视图"""
    def __init__(self):
        super().__init__()
        self.init_ui()
        self.init_signals()

    def init_ui(self):
        # 主布局
        main_layout = QVBoxLayout()

        # 按键布局
        button_layout = QHBoxLayout()
        self.create_button = QPushButton("创建实验")
        self.query_button = QPushButton("查询实验")
        self.details_button = QPushButton("查看详情")
        button_layout.addWidget(self.create_button)
        button_layout.addWidget(self.query_button)
        button_layout.addWidget(self.details_button)
        button_layout.addStretch()

        # 实验列表
        self.table_view = QTableView()
        self.table_view.setSortingEnabled(True)
        self.table_view.horizontalHeader().setSectionResizeMode(QHeaderView.Stretch)

        main_layout.addLayout(button_layout)
        main_layout.addWidget(self.table_view)

        self.setLayout(main_layout)

    def init_signals(self):
        self.create_button.clicked.connect(self.create_experiment)
        self.query_button.clicked.connect(self.query_experiments)
        self.details_button.clicked.connect(self.view_experiment_details)

    def create_experiment(self):
        print("创建实验按钮被点击")

    def query_experiments(self):
        print("查询实验按钮被点击")

    def view_experiment_details(self):
        print("查看详情按钮被点击")
