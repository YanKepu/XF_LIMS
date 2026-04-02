# controllers/experiment_controller.py
from PySide6.QtCore import QObject
from PySide6.QtWidgets import QMessageBox

from common.tcp_client import tcp_client

class ExperimentController(QObject):
    """实验管理控制器"""
    def __init__(self, view):
        super().__init__()
        self._view = view

        # Connect signals from the view to controller methods
        self._view.create_experiment_signal.connect(self._on_create_experiment)

    def _on_create_experiment(self, data):
        """Handle the create experiment signal from the view."""
        cmd = "create_experiment"

        try:
            response = tcp_client.send_request(cmd, data)
            if response and response.get("successflag"):
                self._view.show_message("成功", response.get("msg", "实验创建成功！"))
                self._view.clear_form()
                self._view.switch_to_list_view()
            else:
                self._view.show_message("失败", response.get("msg", "实验创建失败！"), msg_type='warning')
        except ConnectionError as e:
            self._view.show_message("连接错误", str(e), msg_type='critical')
        except Exception as e:
            self._view.show_message("错误", f"发生未知错误: {e}", msg_type='critical')
