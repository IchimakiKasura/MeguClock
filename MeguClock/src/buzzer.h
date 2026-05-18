#pragma once
#include "assets/BuzzerMelodies.h"

extern inline void updateFunction(void (*func)(), uint16_t &ms, uint16_t t);

struct JingleState
{
    int16_t nextTime = 0;
    uint8_t jingle;
    uint8_t index = 0;
    uint8_t delays = 0;
    bool playing = false;
    bool borders = false;
} jingleState;

void quickBeepStart()
{
    tone(
#ifdef CUSTOM_PINS
        BUZZER
#else
        5
#endif
        ,
        1000, 50);
}

void Jingle(uint8_t jingleNumber,
            bool blocking = false,
            bool changeBorders = false,
            uint8_t delays = 0)
{
    uint8_t len = pgm_read_byte_near(&melodies[jingleNumber][0]);

    if (blocking)
    {

        for (uint8_t n = 1, i = 0; i < len; i++, n = 1 + i * 2)
        {
            uint16_t note = pgm_read_word_near(&melodies[jingleNumber][n]);
            uint16_t dur = pgm_read_word_near(&melodies[jingleNumber][n + 1]);

            if (note)
            {
                tone(
#ifdef CUSTOM_PINS
                    BUZZER
#else
                    5
#endif
                    ,
                    note);
                delay(dur + delays);
                noTone(
#ifdef CUSTOM_PINS
                    BUZZER
#else
                    5
#endif
                );
            }
            else
            {
                delay(dur);
            }

            delay(30);
        }
    }
    else
    {
        jingleState = {};   // refreshes
        jingleState.jingle = jingleNumber;
        jingleState.delays = delays;
        jingleState.playing = true;
        jingleState.borders = changeBorders;
    }
}

void updateJingle()
{

    static bool playing = false;
    static uint16_t end = 0;
    static uint16_t lastBorder = 0;

    if (!jingleState.playing)
        return;

    uint8_t len = pgm_read_byte_near(&melodies[jingleState.jingle][0]);

    if (jingleState.borders)
        updateFunction([]()
                       { Draw.TextColorChange(); }, lastBorder, 500);

    if (jingleState.index >= len)
    {
        jingleState.playing = false;

        noTone(
#ifdef CUSTOM_PINS
            BUZZER
#else
            5
#endif
        );

        M_COLORS::Load();
        Draw.ReDraw(rtc.s_now);
        return;
    }

    uint8_t n = 1 + jingleState.index * 2;

    uint16_t note = pgm_read_word_near(&melodies[jingleState.jingle][n]);

    uint16_t dur = pgm_read_word_near(&melodies[jingleState.jingle][n + 1]);

    if (!playing)
    {

        if (note)
            tone(
#ifdef CUSTOM_PINS
                BUZZER
#else
                5
#endif
                ,
                note);

        end = systemTime + dur + jingleState.delays;
        playing = true;
    }

    if (playing && systemTime >= end)
    {

        if (note)
            noTone(
#ifdef CUSTOM_PINS
                BUZZER
#else
                5
#endif
            );

        playing = false;
        jingleState.nextTime = systemTime + 30;
        jingleState.index++;
    }
}