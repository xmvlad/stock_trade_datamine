#! /usr/bin/python

import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

sber_df = pd.read_csv("../../import/candle/sber_2006_2015_candles.txt")


def compute_wnd0(m):
    wnd = np.zeros(m)
    k = 0
    for i in range((1 - m)/2, (m - 1)/2 + 1):
        wnd[k] = ((3.0*m*m - 7.0 - 20.0*i*i)/4.0)/(m*(m*m - 4.0)/3.0)
        k += 1
    return wnd

def compute_wnd1(m):
    wnd = np.zeros(m)
    k = 0
    for i in range((1 - m)/2, (m - 1)/2 + 1):
        c1 = 5.0*(3.0*m*m*m*m - 18.0*m*m + 31.0)*i - 28.0*(3.0*m*m - 7.0)*i*i*i
        c2 = m*(m*m - 1.0)*(3.0*m*m*m*m - 39.0*m*m + 108.0)/15.0
        wnd[k] = c1/c2
        k += 1
    return wnd

def compute_wnd2(m):
    wnd = np.zeros(m)
    k = 0
    for i in range((1 - m)/2, (m - 1)/2 + 1):
        c1 = 12.0*m*i*i - m*(m*m -1)
        c2 = m*m*(m*m - 1.0)*(m*m - 4.0)/30.0
        print "(%f, %f)" % (c1, c2)
        wnd[k] = c1/c2
        k += 1
    return wnd/2.0

def compute_wnd3(m):
    wnd = np.zeros(m)
    k = 0
    for i in range((1 - m)/2, (m - 1)/2 + 1):
        c1 = -(3.0*m*m - 7.0)*i + 20.0*i*i*i
        c2 = m*(m*m - 1.0)*(3.0*m*m*m*m - 39.0*m*m + 108.0)/2520.0
        wnd[k] = c1/c2
        k += 1
    return wnd/6.0

price_series = np.array(sber_df["close"])

offset = 1000
wnd_size = 201
s = price_series[offset - wnd_size: offset]

wnd_0 = compute_wnd0(wnd_size)
wnd_1 = compute_wnd1(wnd_size)
wnd_2 = compute_wnd2(wnd_size)

wnd_3 = compute_wnd3(wnd_size)

coef0 = np.dot(s, wnd_0)
coef1 = np.dot(s, wnd_1)
coef2 = np.dot(s, wnd_2)
coef3 = np.dot(s, wnd_3)

print "(%f, %f, %f, %f)" % (coef0, coef1, coef2, coef3)

x = np.arange((1 - wnd_size)/2, (wnd_size - 1)/2 + 1)
print x
yp = coef0 + coef1*x + coef2*x*x + coef3*x*x*x

plt.figure()
plt.plot(s)
plt.plot(yp)
plt.show()