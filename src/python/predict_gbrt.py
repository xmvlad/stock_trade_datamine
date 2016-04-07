#!/usr/bin/python

import pandas as pd
import numpy  as np
from sklearn.ensemble import GradientBoostingRegressor
from sklearn.ensemble import RandomForestRegressor
from sklearn.cross_validation import train_test_split
from matplotlib import pyplot as plt

def runs_of_ones_count(bits):
  # make sure all runs of ones are well-bounded
    change_count = 0
    for i in xrange(len(bits) - 1):
        if bits[i] != bits[i + 1]:
            change_count += 1
    return change_count


def deviance_plot(est, X_test, y_test, ax=None, label='', train_color='#2c7bb6',
                  test_color='#d7191c', alpha=1.0):
    
    n_estimators = len(est.estimators_)
    
    """Deviance plot for ``est``, use ``X_test`` and ``y_test`` for test error. """
    test_dev = np.empty(n_estimators)

    for i, pred in enumerate(est.staged_predict(X_test)):
       test_dev[i] = est.loss_(y_test, pred)

    if ax is None:
        fig = plt.figure(figsize=(8, 5))
        ax = plt.gca()

    ax.plot(np.arange(n_estimators) + 1, test_dev, color=test_color, label='Test %s' % label,
             linewidth=2, alpha=alpha)
    ax.plot(np.arange(n_estimators) + 1, est.train_score_, color=train_color,
             label='Train %s' % label, linewidth=2, alpha=alpha)
    ax.set_ylabel('Error')
    ax.set_xlabel('n_estimators')
    return test_dev, ax

def feature_importance_plot(est, names):
    fig = plt.figure()
    #ax = fig.add_subplot(111)
    
    indices = np.argsort(est.feature_importances_)
    # plot as bar chart
    plt.barh(np.arange(len(names)), est.feature_importances_[indices])
    plt.yticks(np.arange(len(names)) + 0.25, np.array(names)[indices])
    plt.xlabel('Relative importance')
    plt.legend(loc='upper right')

data_df      = pd.read_csv("../../data/sber_data.txt")
target_df    = pd.read_csv("../../data/sber_target.txt")

#data_df      = pd.read_csv("../../data/gazp_data.txt")
#target_df    = pd.read_csv("../../data/gazp_target.txt")

#data_df      = pd.read_csv("../../data/fsi_data.txt")
#target_df    = pd.read_csv("../../data/fsi_target.txt")

#data_df      = pd.read_csv("../../data/frts_data.txt")
#target_df    = pd.read_csv("../../data/frts_target.txt")

#data_df      = pd.read_csv("../../data/lkoh_data.txt")
#target_df    = pd.read_csv("../../data/lkoh_target.txt")

#data_df      = pd.read_csv("../../data/gmkn_data.txt")
#target_df    = pd.read_csv("../../data/gmkn_target.txt")

#data_df      = pd.read_csv("../../data/vtbr_data.txt")
#target_df    = pd.read_csv("../../data/vtbr_target.txt")

#data_df = pd.merge(data_df, rts_index_df, on=["date", "time"], suffixes=("", ""))
#data_df.to_csv("../../data/df_data.txt", index=False)


#dummy_df_data = { "date": data_df["date"], "time": data_df["time"] }
#dummy_df = pd.DataFrame(dummy_df_data)
#target_df = pd.merge(target_df, dummy_df, on=["date", "time"], suffixes=("", ""))
#target_df.to_csv("../../data/df_target.txt", index=False)

data_df.drop("date", axis=1, inplace=True)
data_df.drop("time", axis=1, inplace=True)

predict_step = 15
data_array = data_df.as_matrix()
data_names = list(data_df.columns.values)

target_price = np.array(target_df[["price"]])
target_time = np.array(target_df[["time"]])
target_date  = np.array(target_df[["date"]])
target_array = np.append(target_price[predict_step:] - target_price[0:-predict_step], [[0]*predict_step,])

q85, q15 = np.percentile(target_array, [85 ,15])
print np.std(target_array)
target_array[target_array > q85] = q85
target_array[target_array < q15] = q15
print np.std(target_array)


subsample = np.arange(0, len(target_price), predict_step)

data_sample = data_array[subsample, :]
target_sample = target_array[subsample]

train_size = 0.8
sample_len = len(target_sample)
train_len = int(train_size*sample_len)
test_len  = int((1.0 - train_size)*sample_len)
data_train   = data_sample[0:train_len,:]
target_train = target_sample[0:train_len]
data_test    = data_sample[train_len:,:]
target_test  = target_sample[train_len:]

#est = GradientBoostingRegressor(n_estimators=100, max_depth=5, learning_rate=0.05, min_samples_leaf=50, subsample=0.8)
#est = GradientBoostingRegressor(n_estimators=100, max_depth=5, learning_rate=0.05, min_samples_leaf=250, subsample=0.8)
est = RandomForestRegressor(n_estimators=100, min_samples_leaf=250, bootstrap=True, oob_score=True, n_jobs=6)
est.fit(data_train, target_train)

full_len = int(len(target_date)*train_size)

pred_train = est.predict(data_train)
pred_test = est.predict(data_test)
pred_test_full = est.predict(data_array[full_len:])

print target_date[full_len:][0:10]
print target_time[full_len:][0:10]
print pred_test_full
print target_price[full_len:][0:10]


pred_data_csv = { "date": target_date[full_len:].flatten(), "time": target_time[full_len:].flatten(),  "pred": pred_test_full, "price": target_price[full_len:].flatten()}
pred_data_df = pd.DataFrame(pred_data_csv)
pred_data_df.to_csv("../../data/pred_data.txt", index=False)


signal_train = 2*(pred_train > 0) - 1
print signal_train
train_equity = np.cumsum(signal_train*target_train)

signal_test = 2*(pred_test > 0) - 1
test_equity = np.cumsum(signal_test*target_test)

#test_dev, ax_dev = deviance_plot(est, data_test, target_test)
#ax_dev.legend(loc='upper right')

feature_importance_plot(est, data_names)

fig = plt.figure()
ax = fig.add_subplot(111)
ax.plot(train_equity, label="train equity")
ax.legend(loc='upper left')

fig = plt.figure()
ax = fig.add_subplot(111)
ax.plot(test_equity, label="test equity")
ax.legend(loc='upper left')

print "test:" 
print test_equity[-1]/len(signal_test)
print len(signal_test)
print np.mean(np.fabs(target_test))

deal_count_train = runs_of_ones_count(signal_train)
print "train:"
print train_equity[-1]/len(signal_train)
print deal_count_train
print np.mean(np.fabs(target_train))

plt.show()
