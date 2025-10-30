from PySide6.QtCore import QObject, Slot
from views.main_view import MainView
from models.tcp_client import TcpClient
from models.sample import Sample
from common.constants import MSG_TYPE_SAMPLE_LIST, MSG_TYPE_SAMPLE_REGISTER, CODE_SUCCESS

class SampleController(QObject):
    def __init__(self, view: MainView, tcp_client: TcpClient, token: str):
        super().__init__()
        self.view = view
        self.tcp_client = tcp_client
        self.token = token  # 登录令牌（权限验证）
        self._init_signals()

    def _init_signals(self):
        """绑定信号：视图事件 → 控制器；服务器消息 → 控制器"""
        # 视图信号（刷新/新增样品）
        self.view.refresh_samples_clicked.connect(self._load_samples)
        self.view.add_sample_clicked.connect(self._open_sample_form)
        # 服务器消息
        self.tcp_client.message_received.connect(self._handle_server_response)

    @Slot()
    def _load_samples(self):
        """加载样品列表：向服务器发送请求"""
        request = {
            "type": MSG_TYPE_SAMPLE_LIST,
            "data": {},
            "token": self.token
        }
        self.tcp_client.send_message(request)

    @Slot(dict)
    def _handle_server_response(self, data: dict):
        """处理服务器响应：更新样品列表"""
        if data.get("type") == MSG_TYPE_SAMPLE_LIST and data.get("code") == CODE_SUCCESS:
            # 解析数据为Sample对象列表
            samples_data = data["data"]["samples"]
            samples = [Sample.from_dict(item) for item in samples_data]
            # 通知视图更新表格
            self.view.update_sample_table(samples)

    def _open_sample_form(self):
        """打开样品登记表单（省略，可参考登录控制器的逻辑）"""
        pass  # 实际开发中需实现表单视图和控制器