# stock_trade_datamine

My hobby stock prediction project

# Perfomance
Out of sample equity perfomance for MOEX Sberbank:
![alt tag](https://raw.githubusercontent.com/xmvlad/stock_trade_datamine/master/img/sber/out_of_sample_test_equity_15106_trades.png)

Although perfomance is quiet good on various assets, model is not tradeabel at least in low-frequency, because average per trade profits comparable to exchange commission

# Description
Compute different factors based on 1-minute candle data. For example, various technical indicators like RSI, ADX, MA, candle stick patterns Doji, Three in rows, etc.
Cubic polynomial fitted with sliding window, and it coeeficients used as prediction factors.
Model is quiet simple, it use shallow(i.e ~100 trees) Random Forest model with 30 minute price delta as target for prediction

./img folder contains train/test dataset perfomance metricks for various Stocks/Features trader on MOEX (Moskow Stock Exchange)

# Build/Run 
To reproduce results, unpack import data in ./import/candle, then build mainapp via CMake and run it ./bin/mainapp to produce training and test data from candles. 
Then run prediction model at ./src/python/predict_grbt.py

