#pragma once

#include <series/value_series.h>

class ExponentialMA: public ValueSeries
{
public:
    ExponentialMA(int period): m_period(period)
    {
        clear();
    }

    virtual void clear()
    {
        m_ema_val = 0.0;
        m_value = 0.0;
        m_count = 0;
    }

    virtual void add_value(double value)
    {
        if (m_count < m_period)
            m_ema_val = (m_count*m_ema_val + value)/(m_count + 1);
        else
            m_ema_val = ((m_period - 1)*m_ema_val + value)/m_period;

        m_value = m_ema_val;
        m_count += 1;
    }

private:
    int    m_period;
    int    m_count;
    double m_ema_val;
};

