# TOUCH4 host tests

These tests compile the actual Yoke driver against mock Arduino and ESP32 UART APIs. The mock device uses the CRC table from `example/ext_periph/mian.c` independently of the driver CRC implementation.

From the Ryzobee repository root, using a host C++17 compiler (for example MinGW):

```text
g++ -std=c++17 -DARDUINO_ARCH_ESP32 -I tests/yoke_touch4 -I ../Yoke/src tests/yoke_touch4/test.cpp -o touch4_test.exe
./touch4_test.exe
```

The Yoke library is expected next to Ryzobee; adjust the include path if needed. Remove the executable afterwards.

Coverage includes command framing and CRC, all command replies, channel masks and raw byte order, invalid input, bad CRC/tails/lengths, truncated frames, timeouts, parser resynchronization, extra firmware version replies, cache retention and port lifecycle. Hardware UART timing and electrical behavior require testing on a connected module.
