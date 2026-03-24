/**************************************************************************
    MEGUMIN CLOCK (MEGUCLOCK / MEGU-CLOCK)

    A custom digital clock built on Arduino, inspired by Megumin.

    Features:
        - Fake loading because god knows its just a simple mcu.
        - Customizable uh colors
        - clock can be adjusted using 2 buttons
        - added shitty buzzer to uh for alarm idfk.
        - alarms are preloaded/hardcoded? idfk lmao
            - 6:00 AM `FNAF`
            - 10:00 AM `Samsung Morning Flower`
            - 12:00 PM `Winchester Chime`
            - 3:00 PM `Winchester Chime`
            - 10:00 PM `Short ahh Twinkle star smh`
        - has random ahh messages every 10 seconds at the bottom. *(can be customized through code ofc)*
        - has an epic border.

    - Includes modified libraries (Reduced Flash Storage)
        - MeguClock_DS3231 (Originally from Adafruit RTCLib)
        - MeguClock_ST7735 (Originally from Adafruit ST7735 & ST7789)

    notes:
        So far, I've used every other way to optimize the flash storage so I can put more pictures
        or even animated boot.

        This might be my ambiguous project that I made using Arduino/C++, its just a simple project
        but It did paved way for me to learn some C++ as it deepends my knowledge about it.

        My most proud moment is where I made the Megumin Logo efficiently as possible by using
        weird algorithms as the img2cpp consumes a lot of flash.

        3/25/26 - Current goal would be reducing from 70% to around 60%? sounds impossible as
        I've used most of the trick to reduce flash, using registers, modified 3rd party libraries
        into barebones. Also less spaghetti code pls.

    Author: Ichimaki Kasura
**************************************************************************/
#include <Arduino.h>
#include "utils/vars.h"

inline void updateFunction(void (*func)(), uint32_t &ms, int16_t t) {
    if (systemTime - ms < t) return;
    func();
    ms = systemTime;
}

void timeUpdate() {
    if (!editMode) {
        if (rtc.s_now.minute() != lastTime.minute() ||
            rtc.s_now.hour()   != lastTime.hour()   ||
            rtc.s_now.day()    != lastTime.day()) {

            Draw.ReDraw();
            lastTime = rtc.s_now;
            quickBeepStart();
        }
        return;
    }

    updateFunction([](){
        Draw.blinkState = !Draw.blinkState;

        if (selected != FIELD_HOUR && !h_edited && rtc.s_now.hour() != rtc.h)
            rtc.h = rtc.s_now.hour();

        if (selected != FIELD_MIN && !m_edited && rtc.s_now.minute() != rtc.m)
            rtc.m = rtc.s_now.minute();

        Draw.ReDraw({rtc.y, rtc.mo, rtc.d, rtc.h, rtc.m, rtc.s_now.second()});
    }, Draw.lastBlink, 700);
}

void bottomTextUpdate() {
    if (jingleState.playing) return;

    uint8_t newIndex;
    do {
        newIndex = rand(46);
    } while (newIndex == lastBottomIndex && 46 > 1);

    lastBottomIndex = newIndex;
    bottomIndex = newIndex;

    Draw.Bottom(strcpy_P(bottomTextBuffer, bottomMessages[newIndex]));
}

void initialize() {
    init();

#ifndef CUSTOM_PINS
    Draw.init(2, 1, 0);
#else
    Draw.init(TFT_CS, TFT_DC, TFT_RST);
    digitalWrite(TFT_RST, HIGH);
#endif
    M_COLORS::Load();
    Draw.SystemBoot();

    Jingle(CHIISANA_BOKENSHA_JINGLE, true);
    delay(100);

    Draw.FakeLoading();

    rtc.init();

#ifndef CUSTOM_PINS
    // some brainfuck shit idfk, it saves like 40bytes?
    // rest pins are input | D5 output
    DDRD = 0x20;    // (1<<5) b00100000
    PORTD = 0x0C;   // (1<<2)|(1<<3) b00001100
#else
    pinMode(BTN_SELECT, INPUT_PULLUP);
    pinMode(BTN_ADJUST, INPUT_PULLUP);
    pinMode(BUZZER, OUTPUT);
#endif

    Draw.bg();

    //mRTC.sync();
}

int main() {
    initialize();

    Draw.Header(0);
    Draw.Bottom(strcpy_P(bottomTextBuffer, bottomMessages[rand(4)]));

    while(true) {
        systemTime = millis();
        
        updateJingle();

        updateFunction([](){Draw.CheckeredBorders();}, borderLastUpdate, 500);
        updateFunction(bottomTextUpdate, lastBottomUpdate, 10000);

        rtc.isRTC = rtc.rtcConnected();

        if (rtc.isRTC) {
            rtc.updateRTC();

            #ifndef CUSTOM_PINS
                adjustHeld = !(PIND & (1 << 3));
            #else
                adjustHeld = digitalRead(BTN_ADJUST);
            #endif

            timeUpdate();

            updateFunction(handleBothButtons, lastCheck, 15);

            CheckAlarm(rtc.s_now.hour(), rtc.s_now.minute(), rtc.s_now.second());
        }

        if (rtc.isRTC && !lastRTCState) 
            Draw.ReDraw();
        else if (!rtc.isRTC && lastRTCState) {
            Draw.ReDraw({0,0,0,0,0,0});
        }

        lastRTCState = rtc.isRTC;
    }

    return 0;
}