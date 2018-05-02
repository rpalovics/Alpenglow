import os
import sys
import pandas as pd
import numpy as np

for i in range(1, 14):
    print("preparing batch", i)
    batch = pd.read_csv("batches/batch_%d_train.dat" % i)
    try:
        os.mkdir("batches/batch_%d_prepared" % i)
    except:
        pass
    (pd.Series(range(batch.user.max() + 1)).astype(str) + ":1").to_csv("batches/batch_%d_prepared/FIXED.x" % i, index=False, header=False)
    (pd.Series(range(batch.item.max() + 1)).astype(str) + ":1").to_csv("batches/batch_%d_prepared/SAMPLED.x" % i, index=False, header=False)
    batch['rand'] = np.random.rand(len(batch))
    batch.sort_values('rand', inplace=True)
    batch.user.to_csv("batches/batch_%d_prepared/FIXED.train" % i, header=False, index=False)
    pd.Series([1]).to_csv("batches/batch_%d_prepared/FIXED.test" % i, header=False, index=False)
    batch.item.to_csv("batches/batch_%d_prepared/SAMPLED.train" % i, header=False, index=False)
    pd.Series([1]).to_csv("batches/batch_%d_prepared/SAMPLED.test" % i, header=False, index=False)
    pd.Series([1 for i in range(len(batch))]).to_csv("batches/batch_%d_prepared/train.libfm" % i, header=False, index=False)
    pd.Series([1]).to_csv("batches/batch_%d_prepared/test.libfm" % i, header=False, index=False)

    test_users = pd.read_csv("batches/batch_%d_test.dat" % i)
    test_users = test_users[test_users.user.isin(batch.user)]
    pd.DataFrame(
        test_users.user.unique()
    ).to_csv("batches/batch_%d_prepared/test_users.txt" % i, index=False, header=False, line_terminator=" ")