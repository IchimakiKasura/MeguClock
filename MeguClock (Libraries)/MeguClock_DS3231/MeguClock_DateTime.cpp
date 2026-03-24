/**************************************************************************
    This library is a modified version of the Adafruit RTCLib's DateTime

    https://github.com/adafruit/RTClib
    
    If you wish to use this feel free

    It is specifically* designed for my simple digital clock as the library
    is flash intensive, I've scrap the unused ones to barebones of the code.
**************************************************************************/
#include "MeguClock_DateTime.h"

static uint8_t conv2d(const char *p) {
  uint8_t v = 0;
  if ('0' <= *p && *p <= '9')
    v = *p - '0';
  return 10 * v + *++p - '0';
}

/*!
    @brief  Constructor from (year, month, day, hour, minute, second).

    This code was from the RTCLib's DateTime
    @warning If the provided parameters are not valid (e.g. 31 February),
           the constructed DateTime will be invalid.
    @param year Either the full year (range: 2000--2099) or the offset from
        year 2000 (range: 0--99).
    @param month Month number (1--12).
    @param day Day of the month (1--31).
    @param hour,min,sec Hour (0--23), minute (0--59) and second (0--59).
*/
DateTime::DateTime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min, uint8_t sec) {
  if (year >= 2000U)
    year -= 2000U;
  yOff = year;
  m = month;
  d = day;
  hh = hour;
  mm = min;
  ss = sec;
}

/*!
    @brief  Memory friendly constructor for generating the build time.

    This version is intended to save RAM by keeping the date and time
    strings in program memory.

    This code was from the RTCLib's DateTime

    @param date Date PROGMEM string, e.g. F("Apr 16 2020").
    @param time Time PROGMEM string, e.g. F("18:34:56").
*/
DateTime::DateTime(const __FlashStringHelper *date, const __FlashStringHelper *time) {
  char buff[11];
  memcpy_P(buff, date, 11);
  yOff = conv2d(buff + 9);
  switch (buff[0]) {
    case 'J': m = (buff[1] == 'a') ? 1 : ((buff[2] == 'n') ? 6 : 7); break;
    case 'F': m = 2; break;
    case 'M': m = buff[2] == 'r' ? 3 : 5; break;
    case 'A': m = buff[2] == 'r' ? 4 : 8; break;
    case 'S': m = 9; break;
    case 'O': m = 10; break;
    case 'N': m = 11; break;
    case 'D': m = 12; break;
  }
  d = conv2d(buff + 4);
  memcpy_P(buff, time, 8);
  hh = conv2d(buff);
  mm = conv2d(buff + 3);
  ss = conv2d(buff + 6);
}

/*!
    @brief  Returns 12 hour clock.

    If the clock is more than 12, e.g. 13, it will return as 1.

    This code was from the RTCLib's DateTime
*/
uint8_t DateTime::twelveHour() const {
  if (hh == 0 || hh == 12) {
    return 12;
  } else if (hh > 12) {
    return hh - 12;
  } else {
    return hh;
  }
}

/*!
    @brief  Gives weekdays as numerics

    `0 = Sunday`, `1 = Monday`, etc...

    This code was from the RTCLib's DateTime
*/
uint8_t DateTime::dayOfTheWeek() const {
  const uint8_t daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30};
  uint16_t y    = yOff,
           days = d;
  if (y >= 2000U)
    y -= 2000U;
  for (uint8_t i = 1; i < m; ++i)
    days += daysInMonth[i - 1];
  if (m > 2 && y % 4 == 0)
    ++days;
  uint16_t day = days + 365 * y + (y + 3) / 4 - 1;
  return (day + 6) % 7;
}

/*!
    @brief  Gives weekdays as const chars*

    `Sunday`, `Monday`, etc...
*/
char* DateTime::dayOfTheWeekName() const {
  switch(dayOfTheWeek()) {
    case 0: return "Sunday";
    case 1: return "Monday";
    case 2: return "Tuesday";
    case 3: return "Wednesday";
    case 4: return "Thursday";
    case 5: return "Friday";
    case 6: return "Saturday";
  }
}

/*!
    @brief  Gives month as const chars*

    `JAN `, `FEB `, etc...

    @note It has trailing space every end of the word.
*/
char* DateTime::monthName() const {
    switch(m){
        case 1:  return "JAN ";
        case 2:  return "FEB ";
        case 3:  return "MAR ";
        case 4:  return "APR ";
        case 5:  return "MAY ";
        case 6:  return "JUN ";
        case 7:  return "JUL ";
        case 8:  return "AUG ";
        case 9:  return "SEP ";
        case 10: return "OCT ";
        case 11: return "NOV ";
        case 12: return "DEC ";
    }
}