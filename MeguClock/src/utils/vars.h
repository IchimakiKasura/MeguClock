#pragma once

bool adjustHeld = false,
     h_edited = false,
     m_edited = false,
     lastRTCState = false;

MeguClock_DS3231 rtc;
DateTime lastTime;

uint8_t bottomIndex,
        lastBottomIndex = -1;

uint16_t systemTime,
         lastBottomUpdate,
         borderLastUpdate,
         lastUpdate,
         lastCheck,
         lastRTCCheck;

const char bottomTextBuffer[20], bottomMessages[][20] PROGMEM = {
    "Compiling reality",
    "Explosion magic!",
    "Debugging time...",
    "System stable(?)",
    "Darkness, lend me",
    "Awaiting interrupt",
    "Prepare to blast!",
    "Firmware mode",
    "Still not working",
    "I cast my spell!",
    "Delay ruins dreams",
    "Measure twice",
    "Stay calibrated",
    "One shot, one kill",
    "Nanoseconds pass",
    "Nothing but boom!",
    "Looping forever",
    "Stack overflow",
    "Magic is supreme",
    "Clock drift again",
    "Resistor missing",
    "Cache invalidated",
    "Reflash maybe?",
    "Chaos reigns now",
    "RTC never lies",
    "Signal detected",
    "Silence = error",
    "Try another pin",
    "Final explosion!",
    "Floating inputs!",
    "Check your GND",
    "Witness my power",
    "Baud mismatch?",
    "Calibrating luck",
    "Hardware issue"
};