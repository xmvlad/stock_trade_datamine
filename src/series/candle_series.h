#pragma once
#include <common/candle.h>
#include "base_series.h"

class CandleSeries: public BaseSeries
{
public:
    virtual void add_candle(const Candle& candle) = 0;
};

