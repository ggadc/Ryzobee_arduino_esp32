# Yoke LCD1D54C 屏幕与触摸示例

使用 240×240 ST7789 屏幕和 CST816T 触摸。

## 功能说明

- 初始化屏幕显示模块和触控功能
- 在屏幕上绘制可点击的红色方块
- 点击方块后随机刷新方块位置
- 点击其他区域时，在触摸点附近绘制绿色像素标记

## 使用说明

安装 Ryzobee、Yoke 和 LovyanGFX 后，编译并烧录 `Yoke_LCD1D54C.ino`。
示例使用 `lcd.begin(board.maker.useport("f_bus"))`，初始化成功返回 `YOKE_PORT_ERROR_NONE`。
LCD1D54C 是可拆卸外设，由示例独立创建和初始化；请勿在同一个 F_BUS 上同时使用其他外设。
触摸复用 `getWire()` 和 `getI2CMutex()`，请保持 board 的生命周期长于 lcd。

| 信号 | F_BUS 引脚 | RootMaker GPIO |
| --- | --- | --- |
| LCD SCK | 11 | 12 |
| LCD RST | 13 | 10 |
| LCD SDA/MOSI | 14 | 9 |
| LCD BL | 16 | 7 |
| LCD CS | 18 | 5 |
| LCD RS/DC | 20 | 3 |
| TP RST | 21 | 2 |
| TP INT | 22 | 1 |
| TP SDA | 23 | 41 |
| TP SCL | 24 | 40 |

`lcd` 继承 LovyanGFX 绘图接口，支持 `setBrightness()`、`fillRect()`、`drawString()` 和 `getTouch(&x, &y)`；`lcd.end()` 关闭背光并释放端口。
`board.maker.begin()` 仅初始化板载外设；四参数版本依次控制 LED、传感器、按键和电池检测。屏幕驱动统一位于 Yoke。
