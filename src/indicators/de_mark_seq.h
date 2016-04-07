#pragma once

#include <deque>

#include <series/candle_series.h>

class DeMarkSeq: public CandleSeries
{
public:
    enum Pattern
    {
        Long,
        Short
    };

    DeMarkSeq(Pattern pattern, int lag, int size): m_pattern(pattern), m_lag(lag), m_size(size) {}


    virtual void clear()
    {
        m_value = 0;
        m_level = 0.0;
        m_window.clear();
    }

    virtual void add_candle(const Candle &candle)
    {
        m_window.push_back(candle.close);

        if (m_window.size() > m_lag + m_size)
            m_window.pop_front();


        if (m_pattern == Long)
        {
            int count = 0;
            for (int i = m_lag; i < m_window.size(); ++i)
            {
                if (m_window[i] < m_window[i - m_lag])
                    count += 1;
            }

            if (count == m_size)
            {
                m_level = candle.close;
            }
        }
        else if (m_pattern == Short)
        {
            int count = 0;
            for (int i = m_lag; i < m_window.size(); ++i)
            {
                if (m_window[i] > m_window[i - m_lag])
                    count += 1;
            }

            if (count == m_size)
            {
                m_level = candle.close;
            }
        }

        m_value = candle.close - m_level;
    }

private:
    std::deque<double> m_window;

    Pattern m_pattern;
    int     m_lag;
    int     m_size;
    double  m_level;
};

