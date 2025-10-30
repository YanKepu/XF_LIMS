class Sample:
    def __init__(self, code: str, name: str, customer: str, status: str = "待检测"):
        self.code = code  # 样品编号
        self.name = name  # 名称
        self.customer = customer  # 客户
        self.status = status  # 状态

    def to_dict(self) -> dict:
        """转为字典，用于JSON序列化"""
        return {
            "code": self.code,
            "name": self.name,
            "customer": self.customer,
            "status": self.status
        }

    @staticmethod
    def from_dict(data: dict):
        """从字典创建Sample对象"""
        return Sample(
            code=data["code"],
            name=data["name"],
            customer=data["customer"],
            status=data.get("status", "待检测")
        )