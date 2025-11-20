# common/json_utils.py
import json
from datetime import datetime
from typing import Any, Dict

class JSONEncoder(json.JSONEncoder):
    """自定义JSON编码器，处理特殊类型"""
    def default(self, obj: Any) -> Any:
        if isinstance(obj, datetime):
            return obj.strftime("%Y-%m-%d %H:%M:%S")
        return super().default(obj)

def json_serialize(data: Any) -> str:
    """序列化数据为JSON字符串"""
    return json.dumps(data, cls=JSONEncoder, ensure_ascii=False)

def json_deserialize(json_str: str) -> Dict[str, Any]:
    """反序列化JSON字符串为字典"""
    try:
        data = json.loads(json_str)
        # 反序列化datetime（如果有）
        for k, v in data.items():
            if isinstance(v, str) and len(v) == 19 and "-" in v and ":" in v:
                try:
                    data[k] = datetime.strptime(v, "%Y-%m-%d %H:%M:%S")
                except ValueError:
                    pass
        return data
    except json.JSONDecodeError as e:
        raise ValueError(f"JSON解析失败: {e}") from e