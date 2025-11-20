# main.spec
# 生成命令：pyinstaller main.spec
import sys
from pathlib import Path
from PyInstaller.utils.hooks import collect_all

# 获取PySide6插件路径（解决Qt界面显示异常）
pyside6_plugins = collect_all("PySide6")[1]

block_cipher = None

a = Analysis(
    ['main.py'],
    pathex=[str(Path(__file__).parent)],
    binaries=[],
    datas=[
        # 包含静态资源（如有）
        ('static', 'static'),
        # 包含配置文件（如有）
        ('config', 'config')
    ],
    hiddenimports=['dataclasses_json', 'loguru'],
    hookspath=[],
    hooksconfig={},
    runtime_hooks=[],
    excludes=[],
    win_no_prefer_redirects=False,
    win_private_assemblies=False,
    cipher=block_cipher,
    noarchive=False,
)

# 添加PySide6插件
for plugin in pyside6_plugins:
    a.datas.append(plugin)

pyz = PYZ(a.pure, a.zipped_data, cipher=block_cipher)

exe = EXE(
    pyz,
    a.scripts,
    a.binaries,
    a.zipfiles,
    a.datas,
    [],
    name='LIMS客户端',  # 生成的exe名称
    debug=False,
    bootloader_ignore_signals=False,
    strip=False,
    upx=True,  # 压缩exe
    upx_exclude=[],
    runtime_tmpdir=None,
    console=False,  # 关闭控制台窗口（GUI程序）
    disable_windowed_traceback=False,
    argv_emulation=False,
    target_arch=None,
    codesign_identity=None,
    entitlements_file=None,
    icon='static/icon.ico'  # 程序图标（可选）
)