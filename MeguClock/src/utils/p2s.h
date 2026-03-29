#pragma once
// Another lightweight sprintf
template<typename T>
inline void p2s(char* out, const char* fmt, T val) {
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
                    const char* str = val;
                    uint8_t len = strlen(str),
                            pad = (len < width) ? (width - len) : 0;
                    while (pad--) *o++ = ' ';
                    while (*str) *o++ = *str++;
                } break;

                case 'd': {
                    for (int v = val, i = width - 1; i >= 0; i--) {
                        o[i] = '0' + (v % 10);
                        v /= 10;
                    }
                    o += width;
                } break;
            }

        } else *o++ = *fmt;
    *o = '\0';
}