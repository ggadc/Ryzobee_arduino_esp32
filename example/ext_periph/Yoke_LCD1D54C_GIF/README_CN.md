# GIF 动画例程

[English](./README.md)

本例程演示如何在 RootMaker 连接的可拆卸 LCD1D54C 屏幕上播放内置 GIF 动画，并通过按键切换不同动画内容。适合用于验证屏幕显示、按键输入以及 GIF 解码播放流程是否正常。

# 功能说明

- 初始化屏幕与按键外设
- 使用 AnimatedGIF 库播放内置 GIF 资源
- 按下按键后切换到下一组 GIF 动画
- 通过串口输出播放状态与调试信息

# 依赖库

本例程依赖以下 Arduino 库，请先在 Arduino IDE 中安装：

| 名称 | 版本 | 说明 |
|---|---|---|
| Yoke | 本地配合库 | 提供 LCD1D54C 屏幕外设驱动 |
| LovyanGFX | >= v1.2.19 | 屏幕绘图接口 |
| [AnimatedGIF](https://github.com/bitbank2/AnimatedGIF) | >= v2.2.0 | 用于在嵌入式设备上解码并播放 GIF 动画 |

# 运行说明

- 下载并安装所需依赖库
- 编译并烧录 example/ext_periph/Yoke_LCD1D54C_GIF/Yoke_LCD1D54C_GIF.ino
- 上电后，屏幕会自动开始播放内置 GIF 动画
- 短按按键可切换到下一组 GIF

# 说明

- GIF 数据已以内置数组形式包含在例程中，无需额外拷贝资源文件
- 本例程启用了屏幕和按键模块，未启用 LED、传感器和电池模块
- 使用 `board.maker.begin(false, false, true, false)` 选择需要启用的板载外设；屏幕通过独立的 Yoke 外设对象初始化。
- 若动画显示异常，请优先检查屏幕初始化、依赖库版本以及编译目标板是否正确

屏幕为可拆卸的 Yoke LCD1D54C 外设，请安装 Yoke、LovyanGFX 和 AnimatedGIF。接线参考 [LCD1D54C 示例](../Yoke_LCD1D54C/README_CN.md)。
