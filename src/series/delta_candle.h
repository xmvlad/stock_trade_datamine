#pragma once

#include "candle_series.h"

class DeltaCandle: public CandleSeries
{
public:
    DeltaCandle(CandleSeries* first_series, CandleSeries* second_series):
        m_first_series(first_series), m_second_series(second_series)
    {
        clear();
    }

    virtual void clear()
    {
        m_first_series->clear();
        m_second_series->clear();
    }

    virtual void add_candle(const Candle& candle)
    {
        m_first_series->add_candle(candle);
        m_second_series->add_candle(candle);
        m_value = m_first_series->value() - m_second_series->value();
    }

private:
    CandleSeries* m_first_series;
    CandleSeries* m_second_series;

};



