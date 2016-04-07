#pragma once

class BaseSeries
{
public:
    BaseSeries(): m_value(0.0) {}
    double value() { return m_value; }
    virtual void clear() = 0;

protected:
    double m_value;
};

