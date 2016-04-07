#pragma once
#include <deque>

#include <series/candle_series.h>

class RunSeq: public CandleSeries
{
public:
    enum Pattern
    {
        Long,
        Short
    };

    RunSeq(Pattern pattern): m_pattern(pattern) { clear(); }


    virtual void clear()
    {
        m_value = 0;
    }

    virtual void add_candle(const Candle &candle)
    {
        if (m_pattern == Long)
        {
            if (candle.close - candle.open > 0.0)
                m_value += 1.0;
            else
                m_value = 0.0;
        }
        else if (m_pattern == Short)
        {
            if (candle.close - candle.open < 0.0)
                m_value += 1.0;
            else
                m_value = 0.0;
        }
    }

private:
    Pattern m_pattern;
};

