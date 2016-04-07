#pragma once

#include <vector>
#include <deque>
#include <common/candle.h>
#include <series/candle_series.h>

class JapanesePattern: public CandleSeries
{
public:
    enum Pattern
    {
        Doji,
        ThreeWhite,
        ThreeBlack,
        HammerWhite,
        HammerBlack,
        StarWhite,
        StarBlack,
        MarubozuWhite,
        MarubozuBlack,

    };

    JapanesePattern(Pattern pattern): m_pattern(pattern)
    {
        clear();
    }

    virtual void clear()
    {
        m_value = 0.0;
        m_level = 0.0;
        m_candles.clear();
    }

    double fill_factor(const Candle& candle)
    {
        if (candle.high - candle.low > 0.0)
        {
            return fabs(candle.close - candle.open)/(candle.high - candle.low);
        }
        else
        {
            return 0.0;
        }
    }

    virtual void add_candle(const Candle& candle)
    {
        m_candles.push_back(candle);

        if (m_candles.size() > 8)
            m_candles.pop_front();
        else
            return;

        if (m_pattern == Doji)
        {
            Candle& candle = m_candles.back();

            if (candle.high - candle.low > 0.0)
            {
                double doji_coef = fabs(candle.close - candle.open)/(candle.high - candle.low);
                if (doji_coef < 0.1)
                    m_level = candle.close;
            }
        }
        else if (m_pattern == ThreeWhite)
        {
            int t = m_candles.size() - 1;
            if (m_candles[t - 0].close - m_candles[t - 0].open > 0.0 &&
                m_candles[t - 1].close - m_candles[t - 1].open > 0.0 &&
                m_candles[t - 2].close - m_candles[t - 2].open > 0.0)
            {
                if (m_candles[t - 1].close > m_candles[t - 2].close &&
                    m_candles[t - 0].close > m_candles[t - 1].close)
                {
                    if (fill_factor(m_candles[t - 2]) > 0.8 &&
                        fill_factor(m_candles[t - 1]) > 0.8 &&
                        fill_factor(m_candles[t - 0]) > 0.8)
                    {
                        m_level = candle.close;
                    }
                }
            }
        }
        else if (m_pattern == ThreeBlack)
        {
            int t = m_candles.size() - 1;
            if (m_candles[t - 0].close - m_candles[t - 0].open < 0.0 &&
                m_candles[t - 1].close - m_candles[t - 1].open < 0.0 &&
                m_candles[t - 2].close - m_candles[t - 2].open < 0.0)
            {
                if (m_candles[t - 1].close < m_candles[t - 2].close &&
                    m_candles[t - 0].close < m_candles[t - 1].close)
                {
                    if (fill_factor(m_candles[t - 2]) > 0.8 &&
                        fill_factor(m_candles[t - 1]) > 0.8 &&
                        fill_factor(m_candles[t - 0]) > 0.8)
                    {
                        m_level = candle.close;
                    }
                }
            }
        }
        else if (m_pattern == HammerWhite)
        {
            if (candle.high - candle.low > 0.0)
            {
                double range = candle.high - candle.low;
                double delta = candle.close - candle.open;
                if (delta > 0.0)
                {
                    if (fabs(candle.close - candle.high)/range > 0.05 &&
                        2.0*fabs(delta) < candle.open - candle.low &&
                        fabs(delta)/range > 0.1)
                    {
                        m_level = candle.close;
                    }
                }
            }
        }
        else if (m_pattern == HammerBlack)
        {
            if (candle.high - candle.low > 0.0)
            {
                double range = candle.high - candle.low;
                double delta = candle.close - candle.open;
                if (delta < 0.0)
                {
                    if (fabs(candle.open - candle.high)/range > 0.05 &&
                        2.0*fabs(delta) < candle.close - candle.low &&
                        fabs(delta)/range > 0.1)
                    {
                        m_level = candle.close;
                    }
                }
            }
        }
        else if (m_pattern == StarWhite)
        {
            if (candle.high - candle.low > 0.0)
            {
                double range = candle.high - candle.low;
                double delta = candle.close - candle.open;
                if (delta > 0.0)
                {
                    if (fabs(candle.open - candle.low)/range > 0.05 &&
                        2.0*fabs(delta) < candle.high - candle.close &&
                        fabs(delta)/range > 0.1)
                    {
                        m_level = candle.close;
                    }
                }
            }
        }
        else if (m_pattern == StarBlack)
        {
            if (candle.high - candle.low > 0.0)
            {
                double range = candle.high - candle.low;
                double delta = candle.close - candle.open;
                if (delta < 0.0)
                {
                    if (fabs(candle.close - candle.low)/range > 0.05 &&
                        2.0*fabs(delta) < candle.high - candle.open &&
                        fabs(delta)/range > 0.1)
                    {
                        m_level = candle.close;
                    }
                }
            }
        }
        else if (m_pattern == MarubozuWhite)
        {
            if (candle.close - candle.open > 0.0 &&
                fill_factor(candle) > 0.9)
            {
                m_level = candle.close;
            }
        }
        else if (m_pattern == MarubozuBlack)
        {
            if (candle.close - candle.open < 0.0 &&
                fill_factor(candle) > 0.9)
            {
                m_level = candle.close;
            }
        }

        m_value = candle.close - m_level;
    }

private:
    Pattern            m_pattern;
    std::deque<Candle> m_candles;
    double             m_level;
};


