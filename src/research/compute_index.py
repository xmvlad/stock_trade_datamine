#! /usr/bin/python

import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from sklearn import linear_model

def suffix_column_names(df, suffix):
    df_columns_map = { "ticker":"ticker" + suffix, "open":"open" + suffix, "close":"close" + suffix, "high":"high" + suffix, "low":"low" + suffix,"volume":"volume"+suffix,"date":"date","time":"time" }
    df.columns = map(lambda x: df_columns_map[x], df.columns)

def moving_average(x, N):
    w = np.array([1.0]*N)
    window = w/sum(w)
    return np.convolve(x, window)[:-(N-1)]


frts_df = pd.read_csv("../../import/candle/frts_2006_2015_candles.txt")
#fix_column_names(frts_df)

fsi_df = pd.read_csv("../../import/candle/fsi_2006_2015_candles.txt")
#fix_column_names(fsi_df)

gazp_df = pd.read_csv("../../import/candle/gazp_2006_2015_candles.txt")
#fix_column_names(gazp_df)

gmkn_df = pd.read_csv("../../import/candle/gmkn_2006_2015_candles.txt")
#fix_column_names(gmkn_df)

lkoh_df = pd.read_csv("../../import/candle/lkoh_2006_2015_candles.txt")
#fix_column_names(lkoh_df)

#mgnt_df = pd.read_csv("./data/MGNT_150401_150601.txt")
#fix_column_names(mgnt_df)

#nvtk_df = pd.read_csv("./data/NVTK_150401_150601.txt")
#fix_column_names(nvtk_df)

#rosn_df = pd.read_csv("./data/ROSN_150401_150601.txt")
#fix_column_names(rosn_df)

sber_df = pd.read_csv("../../import/candle/sber_2006_2015_candles.txt")
#fix_column_names(sber_df)

#sngsp_df = pd.read_csv("./data/SNGSP_150401_150601.txt")
#fix_column_names(sngsp_df)

#sngs_df = pd.read_csv("./data/SNGS_150401_150601.txt")
#fix_column_names(sngs_df)

#trnfp_df = pd.read_csv("./data/TRNFP_150401_150601.txt")
#fix_column_names(trnfp_df)

vtbr_df = pd.read_csv("../../import/candle/vtbr_2007_2015_candles.txt")
#fix_column_names(vtbr_df)

#rtsi_df = pd.read_csv("./data/RTSI_150401_150601.txt")
#fix_column_names(rtsi_df)

suffix_column_names(gazp_df, "_gazp")
suffix_column_names(gmkn_df, "_gmkn")
suffix_column_names(lkoh_df, "_lkoh")
suffix_column_names(sber_df, "_sber")
suffix_column_names(vtbr_df, "_vtbr")

merge_df = pd.merge(frts_df, fsi_df, on=["date", "time"], suffixes=("_rts", "_si"))
merge_df = pd.merge(merge_df, gazp_df, on=["date", "time"], suffixes=("", ""))
merge_df = pd.merge(merge_df, gmkn_df, on=["date", "time"], suffixes=("", ""))
merge_df = pd.merge(merge_df, lkoh_df, on=["date", "time"], suffixes=("", ""))
#merge_df = pd.merge(merge_df, mgnt_df, on=["DATE", "TIME"], suffixes=("", "_MGNT"))
#merge_df = pd.merge(merge_df, nvtk_df, on=["DATE", "TIME"], suffixes=("", "_NVTK"))   #
#merge_df = pd.merge(merge_df, rosn_df, on=["DATE", "TIME"], suffixes=("", "_ROSN"))
merge_df = pd.merge(merge_df, sber_df, on=["date", "time"], suffixes=("", ""))
#merge_df = pd.merge(merge_df, sngsp_df, on=["DATE", "TIME"], suffixes=("", "_SNGSP"))
#merge_df = pd.merge(merge_df, sngs_df, on=["DATE", "TIME"], suffixes=("", "_SNGS"))   #
#merge_df = pd.merge(merge_df, trnfp_df, on=["DATE", "TIME"], suffixes=("", "_TRNFP")) #
merge_df = pd.merge(merge_df, vtbr_df, on=["date", "time"], suffixes=("", ""))
#merge_df = pd.merge(merge_df, rtsi_df, on=["DATE", "TIME"], suffixes=("", "_RTSI"))

merge_df.to_csv("../../data/rts_index_merge_2007_2015.txt", index=False)

rts_series = np.array(merge_df["open_rts"])

si_series = np.array(merge_df["open_si"])
gazp_series = np.array(merge_df["open_gazp"])
gmkn_series = np.array(merge_df["open_gmkn"])
lkoh_series = np.array(merge_df["open_lkoh"])
#mgnt_series = np.array(merge_df["OPEN_MGNT"])
#rosn_series = np.array(merge_df["OPEN_SBER"])
sber_series = np.array(merge_df["open_sber"])
#sngsp_series = np.array(merge_df["OPEN_SNGSP"])
vtbr_series = np.array(merge_df["open_vtbr"])

#nvtk_series = np.array(merge_df["OPEN_NVTK"])
#sngs_series = np.array(merge_df["OPEN_SNGS"])
#trnfp_series = np.array(merge_df["OPEN_TRNFP"])
#rtsi_series = 100.0*np.array(merge_df["OPEN_RTSI"])

linreg = linear_model.LinearRegression(fit_intercept=True)
series_list = [ gazp_series/si_series, gmkn_series/si_series, lkoh_series/si_series, sber_series/si_series, vtbr_series/si_series ]
#series_list = [si_series, gazp_series, gmkn_series, lkoh_series, mgnt_series, rosn_series, sber_series, sngsp_series, vtbr_series, nvtk_series, sngs_series, trnfp_series]
#rts_index_series =  rts_index_series/(0.001*si_series)
#series_list = [np.log(100.0*rts_index_series), np.arange(0, len(rts_index_series)) ]
series_array = np.array(series_list).transpose()
linreg.fit(series_array, np.log(rts_series))

print linreg.coef_
print linreg.intercept_

rts_index = np.exp(linreg.predict(series_array))

residual = rts_series - rts_index

rts_index_data = { "date": merge_df["date"], "time": merge_df["time"],  "index_residual" : residual, "index_residual_ma" : moving_average(residual, 2000)}
rts_index_df = pd.DataFrame(rts_index_data)
rts_index_df.to_csv("../../data/rts_index_2007_2015_data.txt", index=False)

if 0:
    ax1 = plt.subplot(2, 1, 1)
    plt.plot(rts_series)
    plt.plot(rts_index)

    ax2 = plt.subplot(2, 1, 2, sharex=ax1)
    plt.plot(residual)
    plt.plot(moving_average(residual, 2000))
    plt.show()