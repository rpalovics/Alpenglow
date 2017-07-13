import alpenglow as prs
import alpenglow.Getter as rs
import pandas as pd
import numpy as np
import unittest


class TestOfflineModel(unittest.TestCase):
    def test_rmse(self):
        data = pd.read_csv(
            "python/test_alpenglow/test_data_4",
            sep=' ',
            header=None,
            names=['time', 'user', 'item', 'id', 'score', 'eval']
        )
        model = prs.OfflineModel()
        model.fit(data)

        def predict(model, user, item):
            rd = rs.RecDat()
            rd.user = user
            rd.item = item
            return model.prediction(rd)

        errors = [(1 - predict(model.model, u, i))**2 for (u, i) in data[['user', 'item']].values]
        rmse = np.sqrt(pd.Series(errors)).mean()
        self.assertAlmostEqual(0.29222296412604876, rmse)
