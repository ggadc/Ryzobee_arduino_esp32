#pragma once
#include <cstdint>
#include <cstddef>
#include <vector>
#include <string>
#include <deque>
#include <cassert>
#include <cstring>
#include <cstdio>
static uint32_t ticks=0;
inline uint32_t millis() { return ticks++; }
inline void delay(uint32_t ms) { ticks+=ms; }

constexpr int HEX = 16;
class Stream {
public:
    template<typename T> void print(T, int = 10) {}
    template<typename T> void println(T, int = 10) {}
    void println() {}
};
