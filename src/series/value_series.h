#pragma once

#include "base_series.h"

class ValueSeries: public BaseSeries
{
public:
    virtual void add_value(double value) = 0;
};
