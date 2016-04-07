#!/usr/bin/python

import pandas as pd
import numpy  as np
from matplotlib import pyplot as plt

def runs_of_ones_count(bits):
  # make sure all runs of ones are well-bounded
    change_count = 0
    for i in xrange(len(bits) - 1):
        if bits[i] != bits[i + 1]:
            change_count += 1
    return change_count

pred_df      = pd.read_csv("../../data/pred_data.txt")

target_pred = np.array(pred_df[["pred"]])
target_price = np.array(pred_df[["price"]])

target_pred = target_pred - np.mean(target_pred)

price_delta = np.append(target_price[1:] - target_price[0:-1], [0])

signal_list = []

dir = 0
for i in range(len(target_pred)):
    if target_pred[i] > 0.015:
        dir = +1
    elif target_pred[i] < -0.015:
        dir = -1
    signal_list.append(dir)

signal = np.array(signal_list)

equity_delta = price_delta*signal
equity = np.cumsum(equity_delta)

plt.figure()

ax1 = plt.subplot(3, 1, 1)
plt.plot(target_price)

ax2 = plt.subplot(3, 1, 2, sharex=ax1)
plt.plot(target_pred)
plt.plot(signal*0.1)

ax3 = plt.subplot(3, 1, 3, sharex=ax1)
plt.plot(equity)

deal_count = runs_of_ones_count(signal)
print deal_count

print equity[-1]/deal_count

plt.show()