# RAD60 毫米波雷达例程

[English](./README.md)

本例程演示如何在 Ryzobee 板上通过 UART 使用 AT6010（RAD60）毫米波雷达传感器。程序应用默认的运动/呼吸检测参数，并持续上报多目标检测数据（距离、角度、速度、ID）。通信波特率固定为 115200。

# 功能说明

- 通过 `begin(board, port, uart_num, baud)` 在任意开发板扩展口初始化 RAD60
- 按需唤醒 ULP 超低功耗雷达
- 应用默认检测配置（运动 + 呼吸）
- 实时多目标跟踪（最多 2 个目标）
- 无人检测与通信健康监控

# 硬件连接

| Rootmaker 扩展口 | RAD60 模块 |
|-----------------|-----------|
| SDA             | TX        |
| SCL             | RX        |
| VCC             | 3.3V      |
| GND             | GND       |

| 端口   | SDA (TX)  | SCL (RX)  |
|--------|-----------|-----------|
| PORT1  | GPIO15    | GPIO16    |
| PORT2  | GPIO19    | GPIO20    |
| PORT3  | GPIO13    | GPIO14    |
| PORT4  | GPIO17    | GPIO18    |

# 依赖库

无需额外安装第三方 Arduino 依赖库。RAD60 模块使用 ESP-IDF 原生 UART 驱动，与 IDF `at6010` 实现一致。

# 运行说明

- 打开 `examples/ext_periph/Yoke_RAD60/Yoke_RAD60.ino`
- 编译并烧录
- 打开串口监视器，波特率 115200
- 启动约 3 秒后，在雷达前走动即可看到目标信息

# 多端口使用

每个端口创建一个独立实例（每个雷达需要独立 UART）：

```cpp
#include "Ryzobee.h"

Yoke_rad60 radarA, radarB;

void setup() {
    board.rootmaker.begin();
    radarA.begin(RYZOBEE_ROOTMAKER, ROOTMAKER_RS_PORT1, UART_NUM_2, 115200);
    radarB.begin(RYZOBEE_ROOTMAKER, ROOTMAKER_RS_PORT3, UART_NUM_1, 115200);
}

void loop() {
    radarA.update();
    radarB.update();
    if (radarA.isDetected()) {
        Serial.printf("PORT1: %d cm\n", radarA.getTargetDistance(0));
    }
    if (radarB.isDetected()) {
        Serial.printf("PORT3: %d cm\n", radarB.getTargetDistance(0));
    }
    delay(50);
}
```

# 说明

- RAD60 模块需 3.3V 供电并确保电流充足
- 每个 RAD60 需要独立的 UART（UART_NUM_1 = Serial1, UART_NUM_2 = Serial2）
- 通信波特率固定为 115200
