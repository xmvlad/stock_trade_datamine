#include <stdio.h>
#include <common/candle_data.h>
#include <common/text_format.h>

#include <series/candle_adapter.h>
#include <series/dummy_series.h>
#include <series/delta_candle.h>
#include <series/delta_series.h>
#include <series/level_series.h>

#include <indicators/macd.h>
#include <indicators/simple_ma.h>
#include <indicators/exponential_ma.h>
#include <indicators/japanese_pattern.h>
#include <indicators/diffusion_distance.h>
#include <indicators/atr.h>
#include <indicators/de_mark_seq.h>
#include <indicators/run_seq.h>
#include <indicators/rsi.h>
#include <indicators/cubic_poly_filter.h>
#include <indicators/cubic_poly_levels.h>

class SeriesData
{
public:
    CandleSeries*        m_series;
    std::vector<double>  m_data;
    std::string          m_name;

    SeriesData(CandleSeries* series, const std::string name)
    {
        m_series = series;
        m_name = name;
    }

    void clear()
    {
        m_data.clear();
        m_series->clear();
    }

    void push_candle(const Candle& candle)
    {
        m_series->add_candle(candle);
        m_data.push_back(m_series->value());
    }
};

class MainSeries
{
public:
    void setup()
    {
        //add_series("time", new CandleAdapter(new DummySeries(), CandleAdapter::Time));
        add_series("date", new CandleAdapter(new DummySeries(), CandleAdapter::Date));
        add_series("time", new CandleAdapter(new DummySeries(), CandleAdapter::Time));

        add_series("timestamp", new CandleAdapter(new DummySeries(), CandleAdapter::Timestamp));
        //add_series("month_day", new CandleAdapter(new DummySeries(), CandleAdapter::MonthDay));

        setup_sma_delta(60);
        setup_sma_delta(120);
        setup_sma_delta(300);

        setup_sma_macd(60, 120);
        setup_sma_macd(60, 300);

        setup_japanese_pattern();

        //setup_de_mark(4, 2);
        //setup_de_mark(8, 4);
        setup_run_seq();

        setup_rsi_ema();

        setup_cubic_poly_filter(25 + 1);
        setup_cubic_poly_filter(50 + 1);
        setup_cubic_poly_filter(100 + 1);
        setup_cubic_poly_filter(200 + 1);
        setup_cubic_poly_filter(400 + 1);
    }

    void setup_cubic_poly_filter(int size)
    {
        ValueSeries* coef_a0 = new CubicPolyFilter(CubicPolyFilter::A0, size);
        ValueSeries* coef_a1 = new CubicPolyFilter(CubicPolyFilter::A1, size);
        ValueSeries* coef_a2 = new CubicPolyFilter(CubicPolyFilter::A2, size);
        ValueSeries* coef_a3 = new CubicPolyFilter(CubicPolyFilter::A3, size);

        add_series(text_format("cubic_a0_%d", size), new CandleAdapter(coef_a0, CandleAdapter::Close));
        add_series(text_format("cubic_a1_%d", size), new CandleAdapter(coef_a1, CandleAdapter::Close));
        add_series(text_format("cubic_a2_%d", size), new CandleAdapter(coef_a2, CandleAdapter::Close));
        add_series(text_format("cubic_a3_%d", size), new CandleAdapter(coef_a3, CandleAdapter::Close));

        add_series(text_format("cubic_levels1_%d", size), new CubicPolyLevels(CubicPolyLevels::First, coef_a0, coef_a1, coef_a2, coef_a3));
        add_series(text_format("cubic_levels2_%d", size), new CubicPolyLevels(CubicPolyLevels::Second, coef_a0, coef_a1, coef_a2, coef_a3));
    }

    void setup_rsi_ema()
    {
        add_series("rsi_15_ema", new RSI(new ExponentialMA(15), new ExponentialMA(15)));
        //add_series("rsi_30_ema", new RSI(new ExponentialMA(30), new ExponentialMA(30)));
        //add_series("rsi_60_ema", new RSI(new ExponentialMA(60), new ExponentialMA(60)));
        add_series("rsi_120_ema", new RSI(new ExponentialMA(120), new ExponentialMA(120)));
        add_series("rsi_240_ema", new RSI(new ExponentialMA(240), new ExponentialMA(240)));
    }

    void setup_rsi_sma()
    {
        add_series("rsi_15_sma", new RSI(new SimpleMA(15), new SimpleMA(15)));
        add_series("rsi_30_sma", new RSI(new SimpleMA(30), new SimpleMA(30)));
        add_series("rsi_60_sma", new RSI(new SimpleMA(60), new SimpleMA(60)));
        add_series("rsi_120_sma", new RSI(new SimpleMA(120), new SimpleMA(120)));
        add_series("rsi_240_sma", new RSI(new SimpleMA(240), new SimpleMA(240)));
    }

    void setup_run_seq()
    {
        add_series("run_seq_long", new RunSeq(RunSeq::Long));
        add_series("run_seq_short", new RunSeq(RunSeq::Short));
    }

    void setup_de_mark(int lag, int size)
    {
        CandleSeries* de_mark_long = new DeMarkSeq(DeMarkSeq::Long, lag, size);
        add_series(text_format("de_mark_long_%d_%d", lag, size), de_mark_long);

        CandleSeries* de_mark_short = new DeMarkSeq(DeMarkSeq::Short, lag, size);
        add_series(text_format("de_mark_short_%d_%d", lag, size), de_mark_short);

        add_series(text_format("de_mark_delta_%d_%d", lag, size), new DeltaSeries(de_mark_long, de_mark_short));

        /*
        CandleSeries* de_mark_long_4_4 = new DeMarkSeq(DeMarkSeq::Long, 4, 4);
        add_series("de_mark_long_4_4", de_mark_long_4_4);

        CandleSeries* de_mark_long_4_8 = new DeMarkSeq(DeMarkSeq::Long, 4, 8);
        add_series("de_mark_long_4_8", de_mark_long_4_8);

        CandleSeries* de_mark_long_8_2 = new DeMarkSeq(DeMarkSeq::Long, 8, 2);
        add_series("de_mark_long_8_2", de_mark_long_8_2);

        CandleSeries* de_mark_long_8_4 = new DeMarkSeq(DeMarkSeq::Long, 8, 4);
        add_series("de_mark_long_8_4", de_mark_long_8_4);

        CandleSeries* de_mark_long_8_8 = new DeMarkSeq(DeMarkSeq::Long, 8, 8);
        add_series("de_mark_long_8_8", de_mark_long_8_8);
        */


        /*
        CandleSeries* de_mark_short_4_4 = new DeMarkSeq(DeMarkSeq::Short, 4, 4);
        add_series("de_mark_short_4_4", de_mark_short_4_4);

        CandleSeries* de_mark_short_4_8 = new DeMarkSeq(DeMarkSeq::Short, 4, 8);
        add_series("de_mark_short_4_8", de_mark_short_4_8);

        CandleSeries* de_mark_short_8_2 = new DeMarkSeq(DeMarkSeq::Short, 8, 2);
        add_series("de_mark_short_8_2", de_mark_short_8_2);

        CandleSeries* de_mark_short_8_4 = new DeMarkSeq(DeMarkSeq::Short, 8, 4);
        add_series("de_mark_short_8_4", de_mark_short_8_4);

        CandleSeries* de_mark_short_8_8 = new DeMarkSeq(DeMarkSeq::Short, 8, 8);
        add_series("de_mark_short_8_8", de_mark_short_8_8);
        */

    }

    void setup_japanese_pattern()
    {
        add_series("doji", new JapanesePattern(JapanesePattern::Doji));
        add_series("three_white", new JapanesePattern(JapanesePattern::ThreeWhite));
        add_series("three_black", new JapanesePattern(JapanesePattern::ThreeBlack));
        //add_series("hammer_white", new JapanesePattern(JapanesePattern::HammerWhite));
        //add_series("hammer_black", new JapanesePattern(JapanesePattern::HammerBlack));
        add_series("star_white", new JapanesePattern(JapanesePattern::StarWhite));
        add_series("star_black", new JapanesePattern(JapanesePattern::StarBlack));
        add_series("marubozu_white", new JapanesePattern(JapanesePattern::MarubozuWhite));
        add_series("marubozu_black", new JapanesePattern(JapanesePattern::MarubozuBlack));
    }

    void setup_sma_delta(int period)
    {
        CandleSeries* close_series = new CandleAdapter(new DummySeries(), CandleAdapter::Close);
        CandleSeries* sma_series = new CandleAdapter(new SimpleMA(period), CandleAdapter::Close);
        CandleSeries* sma_delta = new DeltaCandle(close_series, sma_series);
        add_series(text_format("sma_delta_%d", period), sma_delta);
    }

    void setup_sma_macd(int fast_period, int slow_period)
    {
        CandleSeries* macd_series = new CandleAdapter(new MACD(new SimpleMA(fast_period), new SimpleMA(slow_period)), CandleAdapter::Close);
        add_series(text_format("macd_sma_%d_%d", fast_period, slow_period), macd_series);
    }

    /*
    void setup()
    {
        CandleComponent* candle_time = new CandleComponent(CandleComponent::Time);
        add_series("time", candle_time);

        CandleComponent* candle_timestamp = new CandleComponent(CandleComponent::Timestamp);
        add_series("timestamp", candle_timestamp);

        CandleComponent* candle_month_day = new CandleComponent(CandleComponent::MonthDay);
        add_series("month_day", candle_month_day);


        setup_sma_macd(60, 120);
        setup_sma_macd(60, 300);

        setup_atr();

        //setup_atr_delta(20, 60);
        //setup_atr_delta(60, 120);
        //setup_atr_delta(60, 300);

        setup_adx(60);
        setup_adx(120);
        setup_adx(300);
    }

    void setup_atr()
    {
        ATR* atr_20 = new ATR(20);
        add_series("atr_20", atr_20);

        ATR* atr_60 = new ATR(60);
        add_series("atr_60", atr_60);

        ATR* atr_120 = new ATR(120);
        add_series("atr_120", atr_120);

        ATR* atr_300 = new ATR(300);
        add_series("atr_300", atr_300);

    }

    void setup_adx(int period)
    {
        ADX* adx = new ADX(period);
        add_series(text_format("adx_%d", period), adx);
    }


    void setup_atr_delta(int small_period, int large_period)
    {
        CandleChain* candle_chain = new CandleChain();
        ATR* atr_small = new ATR(small_period);
        ATR* atr_large = new ATR(large_period);

        DeltaSeries* atr_delta = new DeltaSeries(atr_small, atr_large);

        add_series(text_format("atr_delta_%d_%d", small_period, large_period), atr_delta);
    }

    void setup_sma_delta(int period)
    {
        CandleChain* candle_chain_sma = new CandleChain();
        CandleComponent* candle_close1 = new CandleComponent(CandleComponent::Close);
        SMA* sma = new SMA(period);
        CandleComponent* candle_close2 = new CandleComponent(CandleComponent::Close);

        candle_chain_sma->set_source(candle_close1);
        candle_chain_sma->chain(candle_close1, sma);

        DeltaSeries* sma_delta = new DeltaSeries(candle_close2, candle_chain_sma);

        add_series(text_format("sma_delta_%d", period), sma_delta);
    }

    void setup_sma_macd(int fast_period, int slow_period)
    {
        CandleChain* candle_chain_macd = new CandleChain();
        MACD* macd = new MACD(new SMA(fast_period), new SMA(slow_period));
        CandleComponent* candle_close = new CandleComponent(CandleComponent::Close);

        candle_chain_macd->set_source(candle_close);
        candle_chain_macd->chain(candle_close, macd);

        add_series(text_format("macd_sma_%d_%d", fast_period, slow_period), candle_chain_macd);
    }

    void setup_low_high_channel(int period)
    {
        LowHighChannel* channel_range = new LowHighChannel(period, LowHighChannel::Range);
        add_series(text_format("lhchannel_range_%d", period), channel_range);

        LowHighChannel* channel_low_index = new LowHighChannel(period, LowHighChannel::LowIndex);
        add_series(text_format("lhchannel_low_index_%d", period), channel_low_index);

        LowHighChannel* channel_high_index = new LowHighChannel(period, LowHighChannel::HighIndex);
        add_series(text_format("lhchannel_high_index_%d", period), channel_high_index);
    }
    */

    void add_series(std::string name, CandleSeries* candle_series)
    {
        SeriesData* new_data = new SeriesData(candle_series, name);
        m_series_list.push_back(new_data);
    }

    void process(const std::vector<Candle>& candle_data)
    {
        for (int i = 0; i < candle_data.size(); ++i)
        {
            for (int j = 0; j < m_series_list.size(); ++j)
            {
                m_series_list[j]->push_candle(candle_data[i]);
            }

            m_target_date.push_back(candle_data[i].date);
            m_target_price.push_back(candle_data[i].close);
            m_target_time.push_back(candle_data[i].time);

            if (i % (candle_data.size()/10) == 0)
            {
                printf(".");
                fflush(stdout);
            }
        }
    }

    void save_csv(std::string file_name, const std::vector<std::string>& header, const std::vector<std::vector<double>*>& data)
    {
        FILE* csv_file = fopen(file_name.c_str(), "wt");
        if (csv_file == NULL)
        {
            throw RuntimeError("Can't open file to save output");
        }

        std::string header_str;
        for (int i = 0; i < header.size(); ++i)
        {
            if (i != 0)
                header_str.append(",");

            header_str.append(header[i]);
        }

        fprintf(csv_file, "%s\n", header_str.c_str());

        for (int i = 0; i < data[0]->size(); ++i)
        {
            std::string row_str;

            for (int j = 0; j < data.size(); ++j)
            {
                if (j != 0)
                    row_str.append(",");

                double val = (*data[j])[i];
                row_str += StrUtil::to_str(val);
            }

            fprintf(csv_file, "%s\n", row_str.c_str());
        }
    }

    void save_model(std::string data_file, std::string target_file)
    {
        std::vector<std::string> header;
        std::vector<std::vector<double>* > data;
        for (int i = 0; i < m_series_list.size(); ++i)
        {
            header.push_back(m_series_list[i]->m_name);
            data.push_back(&m_series_list[i]->m_data);
        }

        save_csv(data_file, header, data);

        header.clear();
        data.clear();

        header.push_back("date");
        data.push_back(&m_target_date);
        header.push_back("time");
        data.push_back(&m_target_time);
        header.push_back("price");
        data.push_back(&m_target_price);

        save_csv(target_file, header, data);
    }


private:
    std::vector<SeriesData*> m_series_list;

    std::vector<double>      m_target_date;
    std::vector<double>      m_target_time;
    std::vector<double>      m_target_price;
};

int main_sber()
{
    CandleData candle_data;
    candle_data.load_csv(QUANT_PATH "/import/candle/sber_2006_2015_candles.txt");

    MainSeries main_series;
    main_series.setup();
    main_series.process(candle_data.data());
    main_series.save_model(QUANT_PATH "/data/sber_data.txt",
                           QUANT_PATH "/data/sber_target.txt");

    return 0;
}

int main_gazp()
{
    CandleData candle_data;
    candle_data.load_csv(QUANT_PATH "/import/candle/gazp_2006_2015_candles.txt");

    MainSeries main_series;
    main_series.setup();
    main_series.process(candle_data.data());
    main_series.save_model(QUANT_PATH "/data/gazp_data.txt",
                           QUANT_PATH "/data/gazp_target.txt");

    return 0;
}

int main_si()
{
    CandleData candle_data;
    candle_data.load_csv(QUANT_PATH "/import/candle/fsi_2006_2015_candles.txt");

    MainSeries main_series;
    main_series.setup();
    main_series.process(candle_data.data());
    main_series.save_model(QUANT_PATH "/data/fsi_data.txt",
                           QUANT_PATH "/data/fsi_target.txt");

    return 0;
}

int main_rts()
{
    CandleData candle_data;
    candle_data.load_csv(QUANT_PATH "/import/candle/frts_2006_2015_candles.txt");

    MainSeries main_series;
    main_series.setup();
    main_series.process(candle_data.data());
    main_series.save_model(QUANT_PATH "/data/frts_data.txt",
                           QUANT_PATH "/data/frts_target.txt");

    return 0;
}

int main_lkoh()
{
    CandleData candle_data;
    candle_data.load_csv(QUANT_PATH "/import/candle/lkoh_2006_2015_candles.txt");

    MainSeries main_series;
    main_series.setup();
    main_series.process(candle_data.data());
    main_series.save_model(QUANT_PATH "/data/lkoh_data.txt",
                           QUANT_PATH "/data/lkoh_target.txt");

    return 0;
}

int main_gmkn()
{
    CandleData candle_data;
    candle_data.load_csv(QUANT_PATH "/import/candle/gmkn_2006_2015_candles.txt");

    MainSeries main_series;
    main_series.setup();
    main_series.process(candle_data.data());
    main_series.save_model(QUANT_PATH "/data/gmkn_data.txt",
                           QUANT_PATH "/data/gmkn_target.txt");

    return 0;
}

int main_vtbr()
{
    CandleData candle_data;
    candle_data.load_csv(QUANT_PATH "/import/candle/vtbr_2007_2015_candles.txt");

    MainSeries main_series;
    main_series.setup();
    main_series.process(candle_data.data());
    main_series.save_model(QUANT_PATH "/data/vtbr_data.txt",
                           QUANT_PATH "/data/vtbr_target.txt");

    return 0;
}


int main()
{
    main_sber();
    main_gazp();
    main_si();
    main_rts();
    main_lkoh();
    main_gmkn();
    main_vtbr();
}


