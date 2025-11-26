# common/json_utils.py
import json
import datetime
from typing import Any, Dict, Type, Callable, Optional, Tuple
from decimal import Decimal

# ==============================================================================
#  类型注册器：用于扩展支持的自定义类型
# ==============================================================================

# 序列化注册器: {类型: 转换函数}
# 转换函数: (obj) -> 可被json序列化的基本类型
SERIALIZERS: Dict[Type, Callable[[Any], Any]] = {}

# 反序列化注册器: {类型标识: (目标类型, 转换函数)}
# 转换函数: (value) -> 目标类型对象
DESERIALIZERS: Dict[str, Tuple[Type, Callable[[Any], Any]]] = {}

def register_serializer(type_cls: Type, serializer_func: Callable[[Any], Any]):
    """
    注册一个自定义类型的序列化函数。
    """
    SERIALIZERS[type_cls] = serializer_func

def register_deserializer(type_key: str, type_cls: Type, deserializer_func: Callable[[Any], Any]):
    """
    注册一个自定义类型的反序列化函数。
    type_key 是一个唯一字符串，用于在JSON中标识该类型。
    """
    DESERIALIZERS[type_key] = (type_cls, deserializer_func)


# 注册 date：转为 "YYYY-MM-DD" 字符串
def serialize_date(obj: date) -> str:
    return obj.strftime("%Y-%m-%d")

# 注册 time：转为 "HH:MM:SS" 字符串
def serialize_time(obj: time) -> str:
    return obj.strftime("%H:%M:%S")

# 注册 Decimal：转为字符串（保留完整精度，避免 float 丢失）
def serialize_decimal(obj: Decimal) -> str:
    return str(obj)

# ------------------------------
# 2. 向 SERIALIZERS 注册上述函数
# ------------------------------
SERIALIZERS[datetime] = serialize_datetime
SERIALIZERS[date] = serialize_date
SERIALIZERS[time] = serialize_time
SERIALIZERS[Decimal] = serialize_decimal
SERIALIZERS[Sample] = serialize_sample


# ==============================================================================
#  内置类型支持
# ==============================================================================

# --- datetime ---
DATETIME_TYPE_KEY = "__datetime__"
DATETIME_FORMAT = "%Y-%m-%dT%H:%M:%S.%fZ"  # 使用ISO 8601格式，带时区信息(Z表示UTC)

"""
    序列化时间
"""
def serialize_datetime(obj: datetime.datetime) -> Dict[str, str]:
    # 确保datetime是UTC时间
    if obj.tzinfo is not None and obj.tzinfo.utcoffset(obj) is not None:
        obj_utc = obj.astimezone(datetime.timezone.utc)
    else:
        obj_utc = obj  # 假设是 naive datetime 且代表UTC时间
    return {
        DATETIME_TYPE_KEY: obj_utc.strftime(DATETIME_FORMAT)
    }

# 反序列化时间
def deserialize_datetime(value: Dict[str, str]) -> datetime.datetime:
    dt_str = value[DATETIME_TYPE_KEY]
    dt = datetime.datetime.strptime(dt_str, DATETIME_FORMAT)
    return dt.replace(tzinfo=datetime.timezone.utc)


register_serializer(datetime.datetime, serialize_datetime)
register_deserializer(DATETIME_TYPE_KEY, datetime.datetime, deserialize_datetime)

# --- date ---
DATE_TYPE_KEY = "__date__"
DATE_FORMAT = "%Y-%m-%d"


def serialize_date(obj: datetime.date) -> Dict[str, str]:
    return {
        DATE_TYPE_KEY: obj.strftime(DATE_FORMAT)
    }


def deserialize_date(value: Dict[str, str]) -> datetime.date:
    return datetime.datetime.strptime(value[DATE_TYPE_KEY], DATE_FORMAT).date()


register_serializer(datetime.date, serialize_date)
register_deserializer(DATE_TYPE_KEY, datetime.date, deserialize_date)

# --- Decimal ---
DECIMAL_TYPE_KEY = "__decimal__"


def serialize_decimal(obj: Decimal) -> Dict[str, str]:
    return {
        DECIMAL_TYPE_KEY: str(obj)  # 以字符串形式保存，避免精度丢失
    }


def deserialize_decimal(value: Dict[str, str]) -> Decimal:
    return Decimal(value[DECIMAL_TYPE_KEY])


register_serializer(Decimal, serialize_decimal)
register_deserializer(DECIMAL_TYPE_KEY, Decimal, deserialize_decimal)


# ==============================================================================
#  核心序列化与反序列化逻辑
# ==============================================================================

class LIMSJSONEncoder(json.JSONEncoder):
    """
    LIMS系统专用JSON编码器。
    """

    def default(self, obj: Any) -> Any:
        # 1. 优先使用注册的序列化器
        for type_cls, func in SERIALIZERS.items():
            if isinstance(obj, type_cls):
                return func(obj)

        # 2. 对于自定义对象，可以尝试使用其 __dict__ (谨慎使用)
        #    注意：这可能会暴露过多内部状态，建议只为特定对象开启
        # if hasattr(obj, '__dict__'):
        #     return obj.__dict__

        # 3.  fallback到默认行为，抛出TypeError
        return super().default(obj)


class LIMSJSONDecoder(json.JSONDecoder):
    """
    LIMS系统专用JSON解码器。
    """

    def __init__(self, *args, **kwargs):
        super().__init__(object_hook=self.object_hook, *args, **kwargs)

    def object_hook(self, dct: Dict[str, Any]) -> Any:
        """
        用于将字典转换回自定义对象的钩子函数。
        """
        # 检查字典是否只包含一个键，且该键是已注册的类型标识
        if len(dct) == 1:
            type_key = next(iter(dct.keys()))
            if type_key in DESERIALIZERS:
                type_cls, func = DESERIALIZERS[type_key]
                return func(dct[type_key])

        # 如果不是已知的自定义类型，则返回原始字典
        return dct


# ==============================================================================
#  对外提供的通用函数
# ==============================================================================

def json_serialize(data: Any, **kwargs) -> str:
    """
    将Python对象序列化为JSON字符串。

    Args:
        data: 要序列化的对象。
        **kwargs: 传递给 json.dumps 的额外参数，如 indent, sort_keys等。

    Returns:
        JSON格式的字符串。

    Raises:
        TypeError: 如果遇到无法序列化的对象类型。
    """
    # 强制使用 ensure_ascii=False 以正确处理中文
    kwargs.setdefault('ensure_ascii', False)
    try:
        return json.dumps(data, cls=LIMSJSONEncoder, **kwargs)
    except TypeError as e:
        raise TypeError(f"JSON序列化失败: 无法处理类型 '{type(data).__name__}'。详情: {e}") from e


def json_deserialize(json_str: str, **kwargs) -> Any:
    """
    将JSON字符串反序列化为Python对象。

    Args:
        json_str: JSON格式的字符串。
        **kwargs: 传递给 json.loads 的额外参数。

    Returns:
        反序列化后的Python对象。

    Raises:
        json.JSONDecodeError: 如果JSON字符串格式不正确。
        ValueError: 如果反序列化过程中遇到错误。
    """
    try:
        return json.loads(json_str, cls=LIMSJSONDecoder, **kwargs)
    except json.JSONDecodeError as e:
        raise ValueError(f"JSON反序列化失败: 格式错误。详情: {e}") from e
    except Exception as e:
        raise ValueError(f"JSON反序列化失败: 无法解析内容。详情: {e}") from e
