# MOTO 电机驱动例程

[English](./README.md)

本例程演示如何在 Ryzobee 开发板上使用 MOTO 扩展模块驱动直流电机。模块通过两个 GPIO（IN1/IN2）控制 H 桥电机驱动芯片，支持正转、反转、停止和 PWM 调速。模块与具体开发板解耦，同一份代码可用于 Rootmaker 及后续新型号。

# 功能说明

- 通过 `begin(board, port)` 在任意 Ryzobee 开发板扩展口上初始化 MOTO
- 正转 / 反转 / 停止控制
- 独立的 PWM 速度和方向控制
- 加减速演示（ramp-up / ramp-down）

# 硬件连接

| 扩展口 | 电机驱动 |
|--------|---------|
| SDA    | IN1     |
| SCL    | IN2     |

以 Rootmaker 为例，各端口对应的 GPIO：

| 端口   | SDA (IN1) | SCL (IN2) |
|--------|-----------|-----------|
| PORT1  | GPIO15    | GPIO16    |
| PORT2  | GPIO19    | GPIO20    |
| PORT3  | GPIO13    | GPIO14    |
| PORT4  | GPIO17    | GPIO18    |

# 依赖库

无需额外安装第三方 Arduino 依赖库。

# 运行说明

- 打开 `examples/ext_periph/Yoke_MOTO/Yoke_MOTO.ino`
- 编译并烧录到 Rootmaker 开发板
- 电机会循环执行正转、反转和加减速动作

# 多端口使用

每个端口创建一个独立实例，可同时驱动多个电机：

```cpp
#include "Ryzobee.h"

Yoke_moto motorA, motorB;

void setup() {
    board.rootmaker.begin();
    motorA.begin(RYZOBEE_ROOTMAKER, ROOTMAKER_RS_PORT2);
    motorB.begin(RYZOBEE_ROOTMAKER, ROOTMAKER_RS_PORT3);
}

void loop() {
    motorA.motorForward(200);
    motorB.motorBackward(150);
    delay(1000);
    motorA.motorStop();
    motorB.motorStop();
    delay(500);
}
```

# 说明

- 确保电机驱动板和电机已正确供电
- `motorStop()` 将 IN1/IN2 同时置低（滑行停止）；如需刹车停止，改为同时置高
- `setSpeed()` 仅改变当前方向的 PWM 占空比，不改变方向
