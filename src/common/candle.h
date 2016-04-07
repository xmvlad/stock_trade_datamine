#pragma once
#include <stdint.h>

class Candle
{
public:
    Candle()
    {
        date = 0;
        time = 0;
        open  = 0.0;
        high  = 0.0;
        low   = 0.0;
        close = 0.0;
        volume = 0.0;
    }

    Candle(uint32_t date, uint32_t time, double open, double high,
           double low, double close, double volume)
    {
        this->date = date;
        this->time = time;
        this->open = open;
        this->high = high;
        this->low = low;
        this->close = close;
        this->volume = volume;
    }

    uint32_t date;
    uint32_t time;
    double   open;
    double   high;
    double   low;
    double   close;
    double   volume;
};

