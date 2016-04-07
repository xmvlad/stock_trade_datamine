#pragma once

#include <deque>
#include <vector>
#include <series/value_series.h>

// A0 + A1*x + A2*x*x + A3*x*x*x
class CubicPolyFilter: public ValueSeries
{
public:
    enum Coef
    {
        A0,
        A1,
        A2,
        A3
    };

    CubicPolyFilter(Coef coef, int size): m_size(size)
    {
        clear();

        m_coef.resize(m_size);
        double m = (double)m_size;
        int k = 0;
        for (int i = (1 - m_size)/2; i < (m_size - 1)/2 + 1; ++i)
        {
            if (coef == A0)
            {
                m_coef[k] = ((3.0*m*m - 7.0 - 20.0*i*i)/4.0)/(m*(m*m - 4.0)/3.0);
            }
            else if (coef == A1)
            {
                double c1 = 5.0*(3.0*m*m*m*m - 18.0*m*m + 31.0)*i - 28.0*(3.0*m*m - 7.0)*i*i*i;
                double c2 = m*(m*m - 1.0)*(3.0*m*m*m*m - 39.0*m*m + 108.0)/15.0;
                m_coef[k] = c1/c2;
            }
            else if (coef == A2)
            {
                double c1 = 12.0*m*i*i - m*(m*m -1.0);
                double c2 = m*m*(m*m - 1.0)*(m*m - 4.0)/30.0;
                m_coef[k] = (c1/c2)/2.0;
            }
            else if (coef == A3)
            {
                double c1 = -(3.0*m*m - 7.0)*i + 20.0*i*i*i;
                double c2 = m*(m*m - 1.0)*(3.0*m*m*m*m - 39.0*m*m + 108.0)/2520.0;
                m_coef[k] = (c1/c2)/6.0;
            }

            k += 1;
        }
    }

    virtual void clear()
    {
        m_window.clear();
        m_value = 0.0;
    }

    virtual void add_value(double value)
    {
        m_window.push_back(value);

        if (m_window.size() > m_size)
            m_window.pop_front();

        double v = 0.0;
        for (int i = 0; i < m_window.size(); ++i)
            v += m_coef[i]*m_window[i];

        m_value = v;
    }

private:
    std::vector<double> m_coef;
    std::deque<double>  m_window;
    int m_size;
};


