#pragma once
// goofy lightweight random
inline uint8_t rand(uint8_t num)
{
    static uint16_t seed = 0xACE1u;
    
    seed ^= seed << 7;
    seed ^= seed >> 9;
    seed ^= seed << 8;
    
    return seed % num;
}