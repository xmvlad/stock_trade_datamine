#pragma once

#include <vector>
#include "candle_series.h"
#include "value_series.h"

class LevelSeries: public CandleSeries
{
public:
    LevelSeries(BaseSeries* series): m_series(series) {}

    virtual void clear()
    {
        m_value = 0.0;
    }

    virtual void add_candle(const Candle& candle)
    {
        m_levels.push_back(candle.close);

        double level = m_levels[m_levels.size() - (int)m_series->value() - 1];
        m_value = candle.close - level;
    }


private:
    BaseSeries*         m_series;
    std::vector<double> m_levels;
};


