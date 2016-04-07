#pragma once

#include <series/value_series.h>

class MACD: public ValueSeries
{
public:
    MACD(ValueSeries* fast_series, ValueSeries* slow_series):
        m_fast_series(fast_series), m_slow_series(slow_series)
    {
        clear();
    }

    virtual void clear()
    {
        m_fast_series->clear();
        m_slow_series->clear();
    }

    virtual void add_value(double value)
    {
        m_fast_series->add_value(value);
        m_slow_series->add_value(value);

        m_value = m_fast_series->value() - m_slow_series->value();
    }

private:
    ValueSeries*  m_fast_series;
    ValueSeries*  m_slow_series;
};

