# controllers/experiment_controller.py
from PySide6.QtCore import QObject
from PySide6.QtWidgets import QMessageBox, QTableWidgetItem

from common.tcp_client import tcp_client

class ExperimentController(QObject):
    """实验管理控制器"""
    def __init__(self, view):
        super().__init__()
        self._view = view

        # Connect signals from the view to controller methods
        self._view.create_experiment_signal.connect(self._on_create_experiment)
        
        # Connect search button
        self._view.search_button.clicked.connect(self._on_search_experiments)

    def _on_create_experiment(self, data):
        """Handle the create experiment signal from the view."""
        cmd = "experiment"
        request_data = {
            "action": "create_experiment",
            **data
        }

        try:
            response = tcp_client.send_request(cmd, request_data)
            if response and response.get("code") == 200:
                self._view.show_message("成功", response.get("msg", "实验创建成功！"))
                self._view.clear_form()
                self._view.switch_to_list_view()
            else:
                self._view.show_message("失败", response.get("msg", "实验创建失败！"), msg_type='warning')
        except ConnectionError as e:
            self._view.show_message("连接错误", str(e), msg_type='critical')
        except Exception as e:
            self._view.show_message("错误", f"发生未知错误: {e}", msg_type='critical')

    def _on_search_experiments(self):
        """Handle the search experiment signal from the view."""
        cmd = "experiment"
        search_term = self._view.search_input.text()
        request_data = {
            "action": "get_experiment_list",
            "search_term": search_term,
            "page": 1,
            "page_size": 10
        }

        try:
            response = tcp_client.send_request(cmd, request_data)
            if response and response.get("code") == 200:
                # Clear existing rows
                self._view.experiment_table.setRowCount(0)
                
                # Add new rows from response
                experiments = response.get("experiments", [])
                for experiment in experiments:
                    row_position = self._view.experiment_table.rowCount()
                    self._view.experiment_table.insertRow(row_position)
                    self._view.experiment_table.setItem(row_position, 0, QTableWidgetItem(experiment.get("name", "")))
                    self._view.experiment_table.setItem(row_position, 1, QTableWidgetItem(experiment.get("status", "")))
                    self._view.experiment_table.setItem(row_position, 2, QTableWidgetItem(experiment.get("start_date", "")))
                    self._view.experiment_table.setItem(row_position, 3, QTableWidgetItem(experiment.get("end_date", "")))
                    self._view.experiment_table.setItem(row_position, 4, QTableWidgetItem(""))  # Empty for owner
            else:
                self._view.show_message("失败", response.get("msg", "获取实验列表失败！"), msg_type='warning')
        except ConnectionError as e:
            self._view.show_message("连接错误", str(e), msg_type='critical')
        except Exception as e:
            self._view.show_message("错误", f"发生未知错误: {e}", msg_type='critical')
