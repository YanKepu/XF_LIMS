from PySide6.QtWidgets import (QMainWindow, QTabWidget, QWidget, QVBoxLayout,
                               QHBoxLayout, QPushButton, QTableWidget,
                               QTableWidgetItem, QLabel)
from PySide6.QtCore import Signal

class MainView(QMainWindow):
    # 自定义信号：刷新样品列表、新增样品
    refresh_samples_clicked = Signal()
    add_sample_clicked = Signal()

    def __init__(self):
        super().__init__()
        self.setWindowTitle("实验室管理系统")
        self.resize(800, 600)
        self._init_ui()

    def _init_ui(self):
        """初始化主界面"""
        central_widget = QWidget()
        self.setCentralWidget(central_widget)
        main_layout = QVBoxLayout(central_widget)

        # 标签页
        self.tab_widget = QTabWidget()
        self._init_sample_tab()  # 样品管理标签页
        self.tab_widget.addTab(self.sample_tab, "样品管理")
        # 可添加其他标签页（如任务管理）

        main_layout.addWidget(self.tab_widget)

    def _init_sample_tab(self):
        """初始化样品管理标签页"""
        self.sample_tab = QWidget()
        layout = QVBoxLayout(self.sample_tab)

        # 按钮栏
        btn_layout = QHBoxLayout()
        self.btn_add = QPushButton("新增样品")
        self.btn_add.clicked.connect(self.add_sample_clicked.emit)
        self.btn_refresh = QPushButton("刷新列表")
        self.btn_refresh.clicked.connect(self.refresh_samples_clicked.emit)
        btn_layout.addWidget(self.btn_add)
        btn_layout.addWidget(self.btn_refresh)

        # 样品表格
        self.sample_table = QTableWidget()
        self.sample_table.setColumnCount(4)
        self.sample_table.setHorizontalHeaderLabels(["样品编号", "名称", "客户", "状态"])

        layout.addLayout(btn_layout)
        layout.addWidget(self.sample_table)

    def update_sample_table(self, samples: list):
        """更新样品表格数据（由控制器调用）"""
        self.sample_table.setRowCount(len(samples))
        for row, sample in enumerate(samples):
            self.sample_table.setItem(row, 0, QTableWidgetItem(sample.code))
            self.sample_table.setItem(row, 1, QTableWidgetItem(sample.name))
            self.sample_table.setItem(row, 2, QTableWidgetItem(sample.customer))
            self.sample_table.setItem(row, 3, QTableWidgetItem(sample.status))