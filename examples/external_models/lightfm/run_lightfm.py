import os
import sys
import pandas as pd
import numpy as np
import lightfm
import scipy.sparse as sps
import scipy.sparse.linalg as splinalg

threads = 10

for i in range(1, 14):
    print("running batch %d" % i)
    batch = pd.read_csv("batches/batch_%d_train.dat" % i)
    test_users = pd.read_csv("batches/batch_%d_test.dat" % i)

    model = lightfm.LightFM(
        loss='warp',
        no_components=10,
        learning_rate=0.05,
        learning_schedule="adadelta"
    )
    maxover = batch.groupby('user').item.count().max()
    topk = 100

    def get_ranklists(model, users, items, test):
        import concurrent.futures
        executor = concurrent.futures.ThreadPoolExecutor(threads)

        def predu(i):
            scores = model.predict(
                i,
                items,
                num_threads=1
            )
            return items[np.argsort(scores)[-(topk + maxover):][::-1]]

        preds = list(executor.map(predu, users))
        lists = pd.DataFrame({
            'user': np.repeat(users, topk + maxover),
            'item': np.ndarray.flatten(np.array(preds)),
            'pos': np.tile(np.arange(topk + maxover) + 1, len(users))
        })
        return lists

    uim_train = sps.coo_matrix((np.ones(len(batch)), tuple(zip(*batch[['user', 'item']].values))))
    model = model.fit_partial(
        uim_train,
        epochs=5,
        num_threads=threads,
        verbose=False
    )
    real_test_users = test_users.user[test_users.user.isin(batch.user.unique())].values
    ranklists = get_ranklists(model, real_test_users, batch.item.unique(), None)

    # filtering seen items
    ranklists_j = ranklists.join(batch.set_index(['user', 'item'])['score'], on=['user', 'item'])
    ranklists_j_new = ranklists_j[ranklists_j['score'].isnull()].copy()
    ranklists_j_new.rename(columns={'pos': 'oldpos'}, inplace=True)
    ranklists_j_new.sort_values('oldpos', inplace=True)
    ranklists_j_new['pos'] = 1
    ranklists_j_new['pos'] = ranklists_j_new.groupby('user').pos.transform(np.cumsum)

    ranklists_j_new[ranklists_j_new.pos <= 100][['user', 'item', 'pos']].sort_values(['user','pos']).to_csv('batches/batch_%d_predictions.dat' % i, sep=' ', header=False, index=False)