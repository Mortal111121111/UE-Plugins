@echo off
chcp 65001>nul
setlocal enabledelayedexpansion

REM 创建一个临时文件夹来存放转换后的文件
set tempdir=%cd%\temp
mkdir %tempdir%

REM 遍历当前目录下所有的 .csv 文件
for %%f in (*.csv) do (
    echo 正在转换文件 %%f...

    REM 将文件内容转换为 UTF-8 并存储到临时文件夹中
    type "%%f" > "%tempdir%\%%f"
    powershell -Command "Get-Content '%tempdir%\%%f' | Set-Content -Encoding UTF8 '%%f'"

    REM 删除临时文件
    del "%tempdir%\%%f"
)

REM 删除临时文件夹
rmdir %tempdir%

echo 转换完成！
pause