import json

def json_to_str(data: dict) -> str:
    """将字典转为JSON字符串（加换行符作为分隔符）"""
    return json.dumps(data) + "\n"

def str_to_json(s: str) -> dict:
    """将字符串转为JSON字典"""
    return json.loads(s.strip())