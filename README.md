# 带键盘事件的计算器

这是一个基于 Qt Widgets 和 Qt Designer UI 文件实现的桌面计算器。界面参考
Windows 标准计算器，支持鼠标和键盘输入，并复用同一个计算引擎处理全部操作。

## 功能

- 加、减、乘、除四则运算
- 小数输入与连续小数点过滤
- 退格、清除
- 连续运算符自动替换，避免出现无效表达式
- 连续运算和重复按等号
- 除数为 0 和结果溢出的错误提示
- 计算完成后可继续输入数字或使用结果继续运算
- 支持 `MC`、`M+`、`M-`、`MR` 记忆操作
- 支持百分号输入和常用百分比运算
- 鼠标与键盘输入共用同一套 `CalculatorEngine` 状态逻辑

## 键盘映射

| 键盘 | 功能 |
| --- | --- |
| `0` - `9`、小键盘数字 | 输入数字 |
| `.` 或 `,` | 输入小数点 |
| `+`、`-`、`*`、`/` | 四则运算 |
| `x` 或 `X` | 乘法 |
| `%` | 百分号 |
| `=`、`Enter` | 计算 |
| `Backspace` | 退格 |
| `Esc`、`Delete`、`C` | 清除 |
| `Ctrl+L`、`Ctrl+P`、`Ctrl+M`、`Ctrl+R` | `MC`、`M+`、`M-`、`MR` |

## 项目结构

```text
Lab1.pro                     主程序工程
mainwindow.ui                Qt Designer 界面
mainwindow.h/.cpp            窗口输入分发和显示刷新
calculator_engine.h/.cpp     独立计算状态机
tests/                        Qt Test 边界用例
```

## 构建与运行

在 Qt Creator 中打开 `Lab1.pro`，选择 `Desktop Qt 5.12.11 MinGW 64-bit`
套件后构建运行。

也可以在已配置 Qt 5.12.11 MinGW 环境的终端中执行：

```powershell
qmake Lab1.pro
mingw32-make
.\release\Lab1.exe
```

## 运行测试

```powershell
cd tests
qmake calculator_engine_test.pro
mingw32-make
.\release\calculator_engine_test.exe
```

测试覆盖基础四则运算、连续操作符、连续小数点、退格与清除、除零、结果后继续
输入、重复等号、负数、记忆值、百分号，以及键盘快捷键和鼠标按钮的一致性。
所有键盘和鼠标操作在界面层统一转换为字符串输入，因此测试引擎即可覆盖两种
输入方式的核心行为。

## 验证记录

使用 Qt 5.12.11 MinGW 7.3.0 64-bit 完成验证：

- 主程序 `Lab1.exe` 在启用 `-Wall -Wextra` 时编译成功。
- 计算引擎 Qt Test 共执行 12 个测试项，结果为
  `12 passed, 0 failed, 0 skipped`。
- 鼠标与键盘界面测试共执行 5 个测试项，结果为
  `5 passed, 0 failed, 0 skipped`。
- 测试过程中发现并修复了测试类位于头文件时 qmake 已生成
  `moc_calculator_engine_test.cpp`，源文件无需再包含 `.moc` 的构建问题。

## Git 记录

仓库按“项目骨架、核心实现、测试与文档、问题修复”分阶段提交。可用以下命令查看
迭代历史：

```powershell
git log --oneline --decorate
```
