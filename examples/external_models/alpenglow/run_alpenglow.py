import os
import sys
import pandas as pd
import numpy as np
import lightfm
import alpenglow.offline
from alpenglow.offline.models import FactorModel

threads = 10

for i in range(1, 14):
    print("running batch %d" % i)
    batch = pd.read_csv("batches/batch_%d_train.dat" % i)
    test_users = pd.read_csv("batches/batch_%d_test.dat" % i)
    model = FactorModel(
        negative_rate = 5,
        number_of_iterations = 3,
        learning_rate = 0.05,
    )
    model.fit(batch)
    real_test_users = test_users.user[test_users.user.isin(batch.user.unique())].values
    # ranklists = get_ranklists(model, real_test_users, batch.item.unique(), None)
    ranklists = model.recommend(real_test_users)
    ranklists[['user','item','rank']].to_csv('batches/batch_%d_predictions.dat' % i, sep=' ', header=False, index=False)