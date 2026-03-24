#pragma once
#include "Arduino.h"

class DateTime {
public:
    DateTime() = default;
    DateTime(uint16_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
    DateTime(const __FlashStringHelper*, const __FlashStringHelper*);
    uint16_t year() const { return 2000U + yOff; }
    uint8_t month() const { return m; }
    char* monthName() const;
    uint8_t day() const { return d; }
    uint8_t hour() const { return hh; }
    uint8_t twelveHour() const;
    char* midday() const { return hh >= 12 ? "PM" : "AM"; }
    uint8_t minute() const { return mm >= 59 ? 59 : mm; };
    uint8_t second() const { return ss; };
    uint8_t dayOfTheWeek() const;
    char* dayOfTheWeekName() const;
protected:
    uint8_t yOff;
    uint8_t m;
    uint8_t d;
    uint8_t hh;
    uint8_t mm;
    uint8_t ss;
};