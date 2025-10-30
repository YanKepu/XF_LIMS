from PySide6.QtNetwork import QTcpSocket
from PySide6.QtCore import Signal, QObject
from common.utils import json_to_str, str_to_json
from common.constants import SERVER_IP, SERVER_PORT

class TcpClient(QObject):
    # 自定义信号：接收到消息时触发（传递解析后的JSON数据）
    message_received = Signal(dict)
    # 连接状态变化信号
    connected = Signal()
    disconnected = Signal()
    error_occurred = Signal(str)

    def __init__(self):
        super().__init__()
        self.socket = QTcpSocket()
        self._init_signals()

    def _init_signals(self):
        """绑定QTcpSocket的信号"""
        self.socket.connected.connect(self.connected.emit)
        self.socket.disconnected.connect(self.disconnected.emit)
        self.socket.readyRead.connect(self._on_ready_read)
        self.socket.errorOccurred.connect(
            lambda err: self.error_occurred.emit(self.socket.errorString())
        )

    def connect_to_server(self):
        """连接服务器"""
        self.socket.connectToHost(SERVER_IP, SERVER_PORT)

    def disconnect_from_server(self):
        """断开连接"""
        self.socket.disconnectFromHost()

    def send_message(self, data: dict):
        """发送消息到服务器（自动转为JSON）"""
        if self.socket.state() != QTcpSocket.ConnectedState:
            self.error_occurred.emit("未连接到服务器")
            return
        try:
            msg = json_to_str(data).encode("utf-8")
            self.socket.write(msg)
        except Exception as e:
            self.error_occurred.emit(f"发送失败：{str(e)}")

    def _on_ready_read(self):
        """接收服务器消息并解析"""
        while self.socket.canReadLine():
            try:
                data = self.socket.readLine().data().decode("utf-8")
                json_data = str_to_json(data)
                self.message_received.emit(json_data)
            except Exception as e:
                self.error_occurred.emit(f"解析消息失败：{str(e)}")