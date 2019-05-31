import alpenglow as prs
from alpenglow.offline.models import ALSFactorModel
import alpenglow.Getter as rs
import pandas as pd
import numpy as np
import unittest


class TestALSFactorModel(unittest.TestCase):
    def test_implicit(self):
        data = pd.read_csv(
            "python/test_alpenglow/test_data_4",
            sep=' ',
            header=None,
            names=['time', 'user', 'item', 'id', 'score', 'eval']
        )

        model = ALSFactorModel(
            number_of_iterations=10
        )
        model.fit(data)
        user_items = [(i,j) for i in range(0,data.user.max()) for j in range(0,data.item.max())]
        predictions = model.predict(pd.DataFrame.from_records(user_items, columns=['user','item']))
        user_item_pairs = {(u,i) for u,i in data[['user','item']].values}

        err = 0
        for (s,(u,i)) in zip(predictions, user_items):
            if (u,i) in user_item_pairs:
                err += (s-1)**2
            else:
                err += s**2
        err = (err/len(predictions))**(1/2)

        self.assertAlmostEqual(0.11606286883676992, err)

    def test_explicit(self):
        data = pd.read_csv(
            "python/test_alpenglow/test_data_4",
            sep=' ',
            header=None,
            names=['time', 'user', 'item', 'id', 'score', 'eval']
        )

        model = ALSFactorModel(
            implicit=0,
            number_of_iterations=10
        )
        model.fit(data)
        predictions = model.predict(data[['user','item']])

        err=((np.array(predictions)-1)**2).mean()**(1/2)
        self.assertAlmostEqual(0.02937803875415082, err)
