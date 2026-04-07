# controllers/sample_controller.py
from PySide6.QtCore import QObject
from PySide6.QtWidgets import QMessageBox, QTableWidgetItem

from common.tcp_client import tcp_client

class SampleController(QObject):
    """样品管理控制器"""
    def __init__(self, view):
        super().__init__()
        self._view = view

        # Connect signals from the view to controller methods
        self._view.register_sample_signal.connect(self._on_register_sample)
        
        # Connect search button
        self._view.search_sample_button.clicked.connect(self._on_search_samples)
        
        # Connect register button
        self._view.register_new_sample_button.clicked.connect(self._view._show_sample_registration_view)

    def _on_register_sample(self, data):
        """Handle the register sample signal from the view."""
        cmd = "sample"
        request_data = {
            "action": "register_sample",
            **data
        }

        try:
            response = tcp_client.send_request(cmd, request_data)
            if response and response.get("code") == 200:
                self._view.show_message("成功", response.get("msg", "样品登记成功！"))
                self._view.clear_form()
                self._view.switch_to_list_view()
            else:
                self._view.show_message("失败", response.get("msg", "样品登记失败！"), msg_type='warning')
        except ConnectionError as e:
            self._view.show_message("连接错误", str(e), msg_type='critical')
        except Exception as e:
            self._view.show_message("错误", f"发生未知错误: {e}", msg_type='critical')

    def _on_search_samples(self):
        """Handle the search sample signal from the view."""
        cmd = "sample"
        search_term = self._view.search_sample_input.text()
        request_data = {
            "action": "get_sample_list",
            "search_term": search_term
        }

        try:
            response = tcp_client.send_request(cmd, request_data)
            if response and response.get("code") == 200:
                # Clear existing rows
                self._view.sample_table.setRowCount(0)
                
                # Add new rows from response
                samples = response.get("samples", [])
                for sample in samples:
                    row_position = self._view.sample_table.rowCount()
                    self._view.sample_table.insertRow(row_position)
                    self._view.sample_table.setItem(row_position, 0, QTableWidgetItem(sample.get("code", "")))
                    self._view.sample_table.setItem(row_position, 1, QTableWidgetItem(sample.get("name", "")))
                    self._view.sample_table.setItem(row_position, 2, QTableWidgetItem(sample.get("customer", "")))
                    self._view.sample_table.setItem(row_position, 3, QTableWidgetItem(sample.get("status", "")))
            else:
                self._view.show_message("失败", response.get("msg", "获取样品列表失败！"), msg_type='warning')
        except ConnectionError as e:
            self._view.show_message("连接错误", str(e), msg_type='critical')
        except Exception as e:
            self._view.show_message("错误", f"发生未知错误: {e}", msg_type='critical')
