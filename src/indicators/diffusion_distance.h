#pragma once

#include <float.h>
#include <deque>
#include <series/value_series.h>

class DiffusionDistance: public ValueSeries
{
public:
    DiffusionDistance(BaseSeries* atr, double coef, int depth): m_atr(atr), m_depth(depth), m_coef(coef) { clear(); }

    virtual void clear()
    {
        m_value = 0.0;
        m_points.clear();
    }

    virtual void add_value(double value)
    {
        m_points.push_back(value);

        if (m_points.size() > m_depth)
            m_points.pop_front();
        else
            return;

        double std_val = m_atr->value();
        double min_dist = DBL_MAX;
        for (int i = 0; i < m_depth; ++i)
        {
            if (i > 5)
            {
                double v = (m_points.back() + sqrt(i*m_coef*std_val)) - m_points[m_points.size() - i - 1];
                if (v < min_dist)
                    min_dist = v;
            }
        }

        m_value = min_dist;
    }

private:
    std::deque<double> m_points;
    BaseSeries*        m_atr;
    double             m_coef;
    int m_depth;
};

