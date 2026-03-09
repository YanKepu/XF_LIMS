# 2. 环境配置

本文档描述了如何设置 LIMS 项目的开发和运行环境。

## 2.1. 服务端 (`lims_server`)

- **操作系统**: Ubuntu 20.04
- **编译器**: g++ (支持 C++11)
- **构建系统**: CMake (版本 > 3.10)
- **依赖库**:
  - **Poco**: `sudo apt install libpoco-dev`
  - **libpqxx**: `sudo apt install libpq-dev libpqxx-dev`
  - **nlohmann/json**: 推荐作为子模块或通过 `vcpkg` 管理。为简单起见，可先 `sudo apt install nlohmann-json3-dev`。
  - **libjwt**: `sudo apt install libjwt-dev`
  - **OpenSSL**: `sudo apt install libssl-dev`

### 构建步骤

```bash
cd lims_server
mkdir -p build
cd build
cmake ..
make
```

## 2.2. 客户端 (`lims_client`)

- **操作系统**: Windows
- **Python 版本**: 3.8+
- **编译器&库管理**: Phcharm
- **依赖管理**: 使用 `pip` 和 `requirements.txt`。

### 环境设置步骤

```bash
# 在 Windows 的 cmd 或 PowerShell 中执行
cd lims_client
python -m venv venv
.\venv\Scripts\activate
pip install -r requirements.txt
```
