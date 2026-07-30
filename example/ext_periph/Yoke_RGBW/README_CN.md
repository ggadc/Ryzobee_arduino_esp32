# RGBW LED 例程

[English](./README.md)

本例程演示如何在 Ryzobee 开发板上使用 RGBW 扩展模块控制 NeoPixel RGBW 灯带。模块通过 `begin(board, port, brightness, num_leds)` 自动解析引脚并初始化，循环展示逐像素和全灯色彩填充，包括 RGBW 分量、32 位合成色和延时过渡效果。模块与具体开发板解耦，同一份代码可用于 Rootmaker 及后续新型号。

# 功能说明

- 通过 `begin(board, port, brt, num)` 在任意 Ryzobee 开发板扩展口上初始化 RGBW
- 可配置灯珠数量和亮度
- 逐像素独立设色（RGBW 分量或 32 位合成色）
- 全灯统一设色，支持加延时过渡
- 循环色彩演示

# 硬件连接

| 扩展口 | NeoPixel 灯带 |
|--------|--------------|
| SDA    | 数据线 (DIN) |
| SCL    | 未使用       |

以 Rootmaker 为例，各端口对应的 GPIO：

| 端口   | SDA (DIN) |
|--------|-----------|
| PORT1  | GPIO15    |
| PORT2  | GPIO19    |
| PORT3  | GPIO13    |
| PORT4  | GPIO17    |

# 依赖库

请先在 Arduino IDE 中安装以下库：

| 名称 | 版本 | 说明 |
|---|---|---|
| [Adafruit_NeoPixel](https://github.com/adafruit/Adafruit_NeoPixel) | >= v1.15.3 | 用于控制单总线 RGB / RGBW LED 灯带 |

# 运行说明

- 打开 `examples/ext_periph/Yoke_RGBW/Yoke_RGBW.ino`
- 如需调整亮度和灯珠数量，修改 `begin()` 中的参数
- 编译并烧录到 Rootmaker 开发板
- 灯带会循环展示多种色彩模式

# 多端口使用

每个端口创建一个独立实例：

```cpp
#include "Ryzobee.h"

Yoke_rgbw strip1, strip2;

void setup() {
    board.rootmaker.begin();
    strip1.begin(RYZOBEE_ROOTMAKER, ROOTMAKER_RS_PORT1, 50, 3);
    strip2.begin(RYZOBEE_ROOTMAKER, ROOTMAKER_RS_PORT3, 100, 5);
}

void loop() {
    strip1.setColor(255, 0, 0, 0);   // PORT1: 红色
    strip2.setColor(0, 0, 255, 0);   // PORT3: 蓝色
    delay(500);
    strip1.setColor(0);
    strip2.setColor(0);
    delay(500);
}
```

# 说明

- NeoPixel 数据线为 3.3V 电平，5V 灯带可能需要加电平转换模块
- RGBW 模块仅使用 SDA 引脚（作为数据线），SCL 引脚保留不使用
- 可同时使用多个 RGBW 模块：每个端口创建一个 `Yoke_rgbw` 实例并调用 `begin()`
