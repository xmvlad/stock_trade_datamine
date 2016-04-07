#pragma once

#include <series/value_series.h>
#include <series/candle_series.h>

class RSI: public CandleSeries
{
public:
    RSI(ValueSeries* up_ma, ValueSeries* down_ma): m_up_ma(up_ma), m_down_ma(down_ma) {}


    virtual void clear()
    {
        m_up_ma->clear();
        m_down_ma->clear();
        m_value = 0.0;
        m_prev_close = 0.0;
    }

    virtual void add_candle(const Candle &candle)
    {
        if (candle.close > m_prev_close)
        {
            m_up_ma->add_value(candle.close - m_prev_close);
            m_down_ma->add_value(0.0);
        }
        else
        {
            m_up_ma->add_value(0.0);
            m_down_ma->add_value(m_prev_close - candle.close);
        }

        m_prev_close = candle.close;

        if (m_down_ma->value() != 0.0)
        {
            double rs = m_up_ma->value()/m_down_ma->value();
            m_value = 100.0 - 100.0/(1.0 + rs);
        }
        else
        {
            m_value = 100.0;
        }

    }

private:
    ValueSeries* m_up_ma;
    ValueSeries* m_down_ma;
    double       m_prev_close;
};

