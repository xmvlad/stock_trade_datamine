#pragma once

#include <math.h>
#include <series/candle_series.h>

class ATR: public CandleSeries
{
public:
    ATR(int period): m_period(period)
    {
        clear();
    }

    virtual void clear()
    {
        m_count = 0;
        m_prev_close = 0.0;
        m_atr_first = 0.0;
        m_atr_t = 0.0;
    }

    virtual void add_candle(const Candle &candle)
    {
        double tr;

        if (m_count != 0)
        {
            tr = max(candle.high - candle.low,
                     max(fabs(candle.high - m_prev_close),
                         fabs(candle.low - m_prev_close)));
        }
        else
        {
            tr = max(candle.high - candle.low,
                     max(fabs(candle.high - candle.close),
                         fabs(candle.low - candle.close)));
        }

        if (m_count < m_period)
        {
            m_atr_first += tr;
            m_value = m_atr_first/(m_count + 1);
        }
        else if (m_count == m_period)
        {
            m_atr_first += tr;
            m_atr_t = m_atr_first/m_period;
            m_value = m_atr_t;
        }
        else
        {
            m_atr_t = (m_atr_t * (m_period - 1) + tr)/m_period;
            m_value = m_atr_t;
        }

        m_prev_close = candle.close;
        m_count += 1;
    }

    static double max(double a, double b)
    {
        if (a > b)
            return a;
        else
            return b;
    }

private:
    double   m_prev_close;
    int      m_count;
    int      m_period;
    double   m_atr_first;
    double   m_atr_t;
};

