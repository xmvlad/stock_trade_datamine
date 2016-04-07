#pragma once

#include <deque>
#include <series/value_series.h>

class SimpleMA: public ValueSeries
{
public:
    SimpleMA(int period): m_period(period)
    {
        clear();
    }

    virtual void clear()
    {
        m_window_sum = 0.0;
        m_value = 0.0;
        m_window.clear();
    }

    virtual void add_value(double value)
    {
        if (m_window.size() >= m_period)
        {
            double first_value = m_window.front();
            m_window.pop_front();
            m_window.push_back(value);

            m_window_sum += value - first_value;
            m_value = m_window_sum/m_period;
        }
        else
        {
            m_window.push_back(value);
            m_window_sum += value;
            m_value = m_window_sum/m_window.size();
        }
    }

private:
    std::deque<double>  m_window;
    double              m_window_sum;
    int                 m_period;
};

