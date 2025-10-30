from PySide6.QtCore import QObject, Slot
from views.login_view import LoginView
from models.tcp_client import TcpClient
from common.constants import MSG_TYPE_LOGIN, CODE_SUCCESS

class LoginController(QObject):
    def __init__(self, view: LoginView, tcp_client: TcpClient):
        super().__init__()
        self.view = view  # 关联视图
        self.tcp_client = tcp_client  # 关联TCP模型
        self._init_signals()

    def _init_signals(self):
        """绑定信号：视图事件 → 控制器处理；模型事件 → 控制器处理"""
        # 视图信号（登录按钮点击）
        self.view.login_clicked.connect(self._handle_login)
        # 模型信号（TCP消息接收）
        self.tcp_client.message_received.connect(self._handle_server_response)
        # TCP连接/错误信号
        self.tcp_client.connected.connect(lambda: print("已连接服务器"))
        self.tcp_client.error_occurred.connect(self.view.show_error)

        # 初始连接服务器
        self.tcp_client.connect_to_server()

    @Slot(str, str)
    def _handle_login(self, username: str, password: str):
        """处理登录请求：发送数据到服务器"""
        login_data = {
            "type": MSG_TYPE_LOGIN,
            "data": {
                "username": username,
                "password": password
            }
        }
        self.tcp_client.send_message(login_data)

    @Slot(dict)
    def _handle_server_response(self, data: dict):
        """处理服务器响应：验证登录结果"""
        if data.get("type") != MSG_TYPE_LOGIN:
            return  # 只处理登录相关消息

        if data.get("code") == CODE_SUCCESS:
            # 登录成功：保存令牌，打开主窗口
            self.token = data["data"]["token"]
            self.view.close()
            self._open_main_window()
        else:
            # 登录失败：显示错误
            self.view.show_error(data.get("msg", "登录失败"))

    def _open_main_window(self):
        """打开主窗口（初始化主控制器）"""
        from views.main_view import MainView
        from controllers.sample_controller import SampleController

        self.main_view = MainView()
        self.sample_controller = SampleController(
            view=self.main_view,
            tcp_client=self.tcp_client,
            token=self.token
        )
        self.main_view.show()