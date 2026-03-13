# views/main_window.py
from PySide6.QtWidgets import QMainWindow, QWidget, QHBoxLayout, QListWidget, QStackedWidget, QListWidgetItem
from PySide6.QtCore import Qt, QSize
from PySide6.QtGui import QIcon

from models.user import User
from views.dashboard_view import DashboardView
from views.experiment_view import ExperimentView
from views.sample_view import SampleView
from views.equipment_view import EquipmentView
from views.system_admin_view import SystemAdminView

class MainWindow(QMainWindow):
    """主界面"""
    def __init__(self, user: User):
        super().__init__()
        self.user = user
        self.init_ui()
        self.setup_navigation()

    def init_ui(self):
        # 窗口配置
        self.setWindowTitle(f"LIMS系统 - 主界面（{self.user.username}）")
        self.resize(1280, 800)

        # 中心部件
        central_widget = QWidget()
        self.setCentralWidget(central_widget)

        # 主布局
        main_layout = QHBoxLayout(central_widget)
        main_layout.setContentsMargins(0, 0, 0, 0)
        main_layout.setSpacing(0)

        # 左侧导航栏
        self.nav_bar = QListWidget()
        self.nav_bar.setFixedWidth(180)
        self.nav_bar.setStyleSheet("""
            QListWidget {
                background - color:  # f0f0f0;
                border: none;
                font-size: 14px;
            }
            QListWidget::item
            {
                padding: 15px;
                border-bottom: 1px solid  # dcdcdc;
            }
            QListWidget::item: selected
            {
                background-color:  # cde8ff;
                font-weight: bold;
                color:  # 005a9e;
            }
        """
        )
        main_layout.addWidget(self.nav_bar)

        # 右侧内容区
        self.stacked_widget = QStackedWidget()
        main_layout.addWidget(self.stacked_widget)


    def setup_navigation(self):
        # 创建视图实例
        self.dashboard_view = DashboardView()
        self.experiment_view = ExperimentView()
        self.sample_view = SampleView()
        self.equipment_view = EquipmentView()
        self.system_admin_view = SystemAdminView()

        # 定义导航项
        nav_items = {
            "仪表盘": self.dashboard_view,
            "实验管理": self.experiment_view,
            "样本管理": self.sample_view,
            "设备管理": self.equipment_view,
        }

        # 添加导航项和视图
        for name, view in nav_items.items():
            item = QListWidgetItem(name)
            self.nav_bar.addItem(item)
            self.stacked_widget.addWidget(view)

        # 权限控制：如果用户是管理员，则添加 “系统管理” 模块
        if self.user.username == "admin":
            item = QListWidgetItem("系统管理")
            self.nav_bar.addItem(item)
            self.stacked_widget.addWidget(self.system_admin_view)

        # 连接信号与槽
        self.nav_bar.currentRowChanged.connect(self.stacked_widget.setCurrentIndex)

        # 设置默认选中项
        self.nav_bar.setCurrentRow(0)
