# KEYW 按键模块例程

[English](./README.md)

本例程演示如何在 Ryzobee 开发板上使用 KEYW 扩展模块（按键 + LED）。Yoke 库暂未提供 `Yoke_keyw` 类，因此本例程通过开发板的端口数据向量直接访问 RS 端口引脚。

SDA → 按键输入（低电平有效，内部上拉），SCL → LED 输出（支持 PWM，`analogWrite`）。

# 功能说明

- 从端口数据解析引脚：`board.maker.useport("rs_portN")`
- 下降沿中断 + 30ms 软件去抖
- 每次按下切换 LED 亮灭
- 可通过 `LED_CONTROL` 宏切换数字开关 / PWM 亮度模式

# 硬件连接

| 扩展口 | KEYW 模块 |
|--------|-----------|
| SDA    | 按键输入  |
| SCL    | LED 输出  |

Rootmaker 端口 GPIO 映射：

| 端口   | SDA (按键) | SCL (LED) |
|--------|-----------|-----------|
| PORT1  | GPIO15    | GPIO16    |
| PORT2  | GPIO19    | GPIO20    |
| PORT3  | GPIO13    | GPIO14    |
| PORT4  | GPIO17    | GPIO18    |

# 依赖库

| 名称 | 说明 |
|------|------|
| [Ryzobee](https://github.com/ggadc/Ryzobee_arduino_lib) | 开发板支持（Rootmaker） |

无需 Yoke 库（直接操作 GPIO）。

# 快速开始

```cpp
#define BOARD_RYZOBEE_ROOTMAKER (1)

#include "Ryzobee.h"

Ryzobee board;
std::vector<uint8_t>* keywPort = nullptr;

void setup() {
    board.maker.begin();
    keywPort = board.maker.useport("rs_port1");
    int keyPin = keywPort->at(PORT_PIN_START);      // SDA
    int ledPin = keywPort->at(PORT_PIN_START + 1);  // SCL
    pinMode(keyPin, INPUT_PULLUP);
    pinMode(ledPin, OUTPUT);
    attachInterrupt(digitalPinToInterrupt(keyPin), onKeyPress, FALLING);
}
```

# 说明

- 按键低电平有效，已启用内部上拉，无需外接上拉电阻
- PWM 模式（`LED_CONTROL=1`）使用 `analogWrite`，需确保 SCL 引脚支持 PWM
- Yoke 库后续可能会加入专用的 `Yoke_keyw` 类
