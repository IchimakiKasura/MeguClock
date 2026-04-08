#pragma once
#include <stdarg.h>
// Another lightweight sprintf
inline void p2s(char* out, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    char* o = out;
    for (; *fmt; fmt++)
        if (*fmt == '%') {
            fmt++;

            uint8_t width = 0;
            while (*fmt >= '0' && *fmt <= '9') {
                width = width * 10 + (*fmt - '0');
                fmt++;
            }

            switch (*fmt) {
                // scope bug if i didn't add brackets
                case 's': {
                    const char* str = va_arg(args, const char*);
                    uint8_t len = strlen(str),
                            pad = (len < width) ? (width - len) : 0;
                    while (pad--) *o++ = ' ';
                    while (*str) *o++ = *str++;
                } break;

                case 'd': {
                    for (int v = va_arg(args, int), i = width - 1; i >= 0; i--) {
                        o[i] = '0' + (v % 10);
                        v /= 10;
                    }
                    o += width;
                } break;
            }

        } else *o++ = *fmt;
    va_end(args);
    *o = '\0';
}