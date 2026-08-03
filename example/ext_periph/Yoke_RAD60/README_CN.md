# RAD60 毫米波雷达例程

[English](./README.md)

本例程演示如何使用 **[Yoke](https://github.com/ggadc/Yoke_arduino_lib)** 库的 `Yoke_rad60` 类在 Ryzobee 开发板上驱动 AT6010（RAD60）毫米波雷达传感器。通过 RS 扩展口的 UART 通信（SDA=TX, SCL=RX），波特率固定 115200。

# 功能说明

- 在任意 RS 端口初始化：`radar.begin(board.maker.useport("rs_portN"))`
- 构造时指定 UART：`Yoke_rad60 radar(UART_NUM_1, 115200)`
- 唤醒 ULP 雷达：`radar.enterCommunicationMode(true)`
- 应用默认检测配置（移动 + 呼吸）：`radar.applyDefaultConfig()`
- 实时多目标追踪（最多 2 个）：`radar.update()`
- 每目标输出距离、角度、速度、ID
- 空闲/通信健康监控：`isDetected()`, `isConnected()`

# 硬件连接

| RS 端口 | RAD60 模块 |
|---------|-----------|
| SDA     | TX        |
| SCL     | RX        |
| VCC     | 3.3V      |
| GND     | GND       |

Rootmaker 端口 GPIO 映射：

| 端口   | SDA (TX)  | SCL (RX)  |
|--------|-----------|-----------|
| PORT1  | GPIO15    | GPIO16    |
| PORT2  | GPIO19    | GPIO20    |
| PORT3  | GPIO13    | GPIO14    |
| PORT4  | GPIO17    | GPIO18    |

# 依赖库

| 名称 | 说明 |
|------|------|
| [Ryzobee](https://github.com/ggadc/Ryzobee_arduino_lib) | 开发板支持（Rootmaker） |
| [Yoke](https://github.com/ggadc/Yoke_arduino_lib) | 外设驱动（基于 UART，无需第三方库） |

# 快速开始

```cpp
#define BOARD_RYZOBEE_ROOTMAKER (1)

#include "Ryzobee.h"
#include <ryzobee_yoke.hpp>
#include <yoke_rad60/yoke_rad60.hpp>

Ryzobee board;
Yoke_rad60 radar(UART_NUM_1, 115200);

void setup() {
    board.maker.begin();
    radar.begin(board.maker.useport("rs_port4"));
    delay(2000);                                 // 等待雷达启动
    radar.enterCommunicationMode(true);
    radar.applyDefaultConfig();
}

void loop() {
    radar.update();
    if (radar.isDetected()) {
        Serial.printf("目标数: %d, 距离: %d cm\n",
                      radar.getTargetCount(),
                      radar.getTargetDistance(0));
    }
    delay(50);
}
```

# 说明

- 每个 RAD60 需占用一个独立 UART（UART_NUM_1 或 UART_NUM_2）
- 上电后需等待约 2 秒再发送命令
- 通信波特率固定 115200
- 协议：0x58 发送帧 / 0x59 响应帧 / 0x5A 自动上报帧
