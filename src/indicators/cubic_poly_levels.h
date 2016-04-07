#pragma once

#include <series/value_series.h>
#include <series/candle_series.h>

class CubicPolyLevels: public CandleSeries
{
public:
    enum Root
    {
        First,
        Second
    };

    CubicPolyLevels(Root root, ValueSeries* coef0, ValueSeries* coef1,
                    ValueSeries* coef2, ValueSeries* coef3):
        m_root(root), m_coef0(coef0), m_coef1(coef1), m_coef2(coef2), m_coef3(coef3)
    {
        clear();
    }

    virtual void clear()
    {
        m_value = 0;
        m_coef0->clear();
        m_coef1->clear();
        m_coef2->clear();
        m_coef3->clear();
    }

    // a1 + 2*a2*x + 3*a3*x*x
    virtual void add_candle(const Candle& candle)
    {
        double c = m_coef1->value();
        double b = 2.0*m_coef2->value();
        double a = 3.0*m_coef3->value();

        double d = b*b - 4.0*a*c;
        if (d >= 0.0)
        {
            double x;
            if (fabs(a) == 0.0)
            {
                double g;
                if (m_root == First)
                {
                    g = (b + sqrt(d));
                    if (fabs(g) > 0.0)
                        x = -2.0*c/g;
                    else
                        x = 0.0;
                }
                else if (m_root == Second)
                {
                    g = (b - sqrt(d));
                    if (fabs(g) > 0.0)
                        x = -2.0*c/g;
                    else
                        x = 0.0;
                }
            }
            else
            {
                if (m_root == First)
                    x = (-b + sqrt(d))/(2.0*a);
                else if (m_root == Second)
                    x = (-b - sqrt(d))/(2.0*a);
            }

            double y = m_coef0->value() + x*m_coef1->value() + x*x*m_coef2->value() +
                x*x*x*m_coef3->value();
            m_value = candle.close - y;
        }
        else
        {
            m_value = 0.0;
        }
    }

private:
    Root         m_root;
    ValueSeries* m_coef0;
    ValueSeries* m_coef1;
    ValueSeries* m_coef2;
    ValueSeries* m_coef3;
};
