# KEYW 按键模块例程

[English](./README.md)

本例程演示如何在 Ryzobee 开发板上使用 KEYW 扩展模块（按键 + LED）。模块通过 `begin(board, port)` 自动解析引脚，按键输入采用内部上拉、下降沿中断加软件去抖，LED 支持开关控制和 PWM 亮度调节。模块与具体开发板解耦，同一份代码可用于 Rootmaker 及后续新型号。

# 功能说明

- 通过 `begin(board, port)` 在任意 Ryzobee 开发板扩展口上初始化 KEYW
- 检测按键按下并做软件去抖（30ms）
- 每次按下切换 LED 亮灭状态
- 支持数字开关和 PWM 亮度两种控制模式

# 硬件连接

| 扩展口 | KEYW 模块 |
|--------|-----------|
| SDA    | 按键输入  |
| SCL    | LED 输出  |

以 Rootmaker 为例，各端口对应的 GPIO：

| 端口   | SDA (按键) | SCL (LED) |
|--------|-----------|-----------|
| PORT1  | GPIO15    | GPIO16    |
| PORT2  | GPIO19    | GPIO20    |
| PORT3  | GPIO13    | GPIO14    |
| PORT4  | GPIO17    | GPIO18    |

# 依赖库

无需额外安装第三方 Arduino 依赖库。

# 运行说明

- 打开 `examples/ext_periph/Yoke_KEYW/Yoke_KEYW.ino`
- 编译并烧录到 Rootmaker 开发板
- 打开串口监视器，波特率 115200
- 按下按键，LED 应切换亮灭

# 多端口使用

每个端口创建一个独立实例，可同时使用多个 KEYW 模块：

```cpp
#include "Ryzobee.h"

Yoke_keyw k2, k4;

void setup() {
    board.rootmaker.begin();
    k2.begin(RYZOBEE_ROOTMAKER, ROOTMAKER_RS_PORT2);
    k4.begin(RYZOBEE_ROOTMAKER, ROOTMAKER_RS_PORT4);
}

void loop() {
    if (k2.readKeyState()) k2.setLedBrightness(128);
    if (k4.readKeyState()) k4.setLedState(true);
}
```

# 说明

- 按键为低电平有效，已启用内部上拉电阻，无需外接上拉
- PWM 模式（`LED_CONTROL = 1`）使用 `analogWrite`，需确保对应的 SCL 引脚支持 PWM
- 若按键无响应，请检查接线和端口选择是否正确
