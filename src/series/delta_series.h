#pragma once

#include "candle_series.h"

class DeltaSeries: public CandleSeries
{
public:
    DeltaSeries(BaseSeries* first_series, BaseSeries* second_series):
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
        m_value = m_first_series->value() - m_second_series->value();
    }

private:
    BaseSeries* m_first_series;
    BaseSeries* m_second_series;
};

