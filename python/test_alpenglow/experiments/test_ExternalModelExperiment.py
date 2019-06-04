import alpenglow as prs
import alpenglow.Getter as rs
import alpenglow.experiments
import pandas as pd
import math
import os
import numpy as np


class TestExternalModelExperiment:
    def test_externalModelExperiment(self):
        try:
            os.mkdir("python/test_alpenglow/tmp")
        except:
            pass
        data = pd.read_csv(
            "python/test_alpenglow/test_data_4",
            sep=' ',
            header=None,
            names=['time', 'user', 'item', 'id', 'score', 'eval']
        )
        experiment = alpenglow.experiments.ExternalModelExperiment(
            period_length=200,
            period_mode="samplenum",
            out_name_base="python/test_alpenglow/tmp/batch",
            mode='write'
        )
        num = 5
        experiment.run(data, initialize_all=True) #initall=true->do not filter out very new items
        trains = [pd.read_csv('python/test_alpenglow/tmp/batch_%d_train.dat' % i) for i in range(1, num + 1)]
        tests = [pd.read_csv('python/test_alpenglow/tmp/batch_%d_test.dat' % i) for i in range(num)]
        last_ids = []
        for i in range(num):
            assert set(tests[i].user.unique()) == set(trains[i][~trains[i].id.isin(last_ids)].user.unique())
            last_ids = trains[i].id.unique()

        empty = pd.DataFrame({
            'user': [],
            'item': [],
            'pos': [],
        })
        for i in range(5):
            empty.to_csv('python/test_alpenglow/tmp/batch_%d_predictions.dat' % i, sep=" ", header=False, index=False)

        df = pd.DataFrame({
            'user': np.repeat([3], 100),
            'item': np.repeat([10000], 100),
            'pos': np.arange(100)
        })
        df.loc[(df.user == 3) & (df.pos == 9), 'item'] = 94
        df[['user', 'item', 'pos']].to_csv('python/test_alpenglow/tmp/batch_1_predictions.dat', sep=" ", header=False, index=False)

        experiment = alpenglow.experiments.ExternalModelExperiment(
            period_length=200,
            period_mode="samplenum",
            in_name_base="python/test_alpenglow/tmp/batch",
            mode='read'
        )
        res = experiment.run(data, exclude_known=True)
        print(res['rank'].isnull().value_counts())
        assert (~res['rank'].isnull()).astype(np.int64).sum() == 1
