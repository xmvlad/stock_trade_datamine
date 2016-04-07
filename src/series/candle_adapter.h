#pragma once

#include "candle_series.h"
#include "value_series.h"

class CandleAdapter: public CandleSeries
{
public:
    enum Component
    {
        Open,
        High,
        Low,
        Close,
        Volume,
        Timestamp,
        Time,
        Date,
        Day,
        MonthDay
    };

    CandleAdapter(ValueSeries* series, Component component): m_series(series), m_component(component) {}

    virtual void clear()
    {
        m_value = 0.0;
    }

    virtual void add_candle(const Candle &candle)
    {
        double value;
        switch (m_component)
        {
        case Open:
            value = candle.open;
            break;

        case High:
            value = candle.high;
            break;

        case Low:
            value = candle.low;
            break;

        case Close:
            value = candle.close;
            break;

        case Volume:
            value = candle.volume;
            break;

        case Time:
            value = candle.time;
            break;

        case Date:
            value = candle.date;
            break;

        case Timestamp:
        {
            int hours   = candle.time/(100*100);
            int minutes = (candle.time / 100) % 100;
            int seconds = candle.time % 100;
            value = 60.0*60.0*hours + 60.0*minutes + seconds;
            break;
        }

        case Day:
        {
            value = candle.date % 100;
            break;
        }

        case MonthDay:
        {
            value = candle.date % 10000;
            break;
        }

        }

        m_series->add_value(value);
        m_value = m_series->value();
    }

private:
    Component    m_component;
    ValueSeries* m_series;
};

