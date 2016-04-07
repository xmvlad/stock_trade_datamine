#pragma once

#include "value_series.h"

class DummySeries: public ValueSeries
{
public:
    DummySeries()
    {
        clear();
    }

    virtual void clear()
    {
        m_value = 0;
    }

    virtual void add_value(double value)
    {
        m_value = value;
    }
};


