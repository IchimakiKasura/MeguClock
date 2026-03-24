#pragma once
#include <Wire.h>
#include "Arduino.h"
#include "MeguClock_DateTime.h"

#define W_START Wire.beginTransmission(0x68)
#define W_STOP Wire.endTransmission()

class MeguClock_DS3231 {
public:
    // temporary placeholder for editables
    uint16_t h, m, mo, d, y;

    bool isRTC;
    
    void init();
    bool rtcConnected();

    DateTime now();
    void adjust(const DateTime&);

    void Load();
    void Save();

    // static now
    static DateTime s_now;

    void updateRTC();
protected:
    static uint8_t bcd2bin(uint8_t val) { return val - 6 * (val >> 4); }
    static uint8_t bin2bcd(uint8_t val) { return val + 6 * (val / 10); }
    static uint8_t dowToDS3231(uint8_t d) { return d == 0 ? 7 : d; }
};