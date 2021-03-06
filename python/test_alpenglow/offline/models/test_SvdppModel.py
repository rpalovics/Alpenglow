import alpenglow as prs
from alpenglow.offline.models import SvdppModel
from alpenglow.offline.evaluation import NdcgScore
import alpenglow.Getter as rs
import pandas as pd
import numpy as np
import unittest
import pytest
import sys
import alpenglow.cpp
compiler = alpenglow.cpp.__compiler
stdlib = alpenglow.cpp.__stdlib

class TestSvdppModel(unittest.TestCase):
    def test_rmse(self):
        data = pd.read_csv(
            "python/test_alpenglow/test_data_4",
            sep=' ',
            header=None,
            names=['time', 'user', 'item', 'id', 'score', 'eval']
        )
        model = SvdppModel(
            negative_rate=9,
            number_of_iterations=20,
        )
        model.fit(data)

        def predict(model, user, item):
            rd = rs.RecDat()
            rd.user = user
            rd.item = item
            return model.prediction(rd)

        errors = [(1 - predict(model.model, u, i))**2 for (u, i) in data[['user', 'item']].values]
        rmse = np.sqrt(pd.Series(errors)).mean()
        assert rmse == pytest.approx(0.38152996215217494, abs=5*1e-3)

    def test_ranking(self):
        data = pd.read_csv(
            "python/test_alpenglow/test_data_4",
            sep=' ',
            header=None,
            names=['time', 'user', 'item', 'id', 'score', 'eval']
        )
        exp = SvdppModel(
            negative_rate=9,
            number_of_iterations=20,
        )
        exp.fit(data)
        preds = exp.recommend(exclude_known=False, k=20)
        if(compiler == "gcc" and stdlib == "libstdc++"):
            #print(preds['item'].tolist())
            assert preds['item'].tolist() ==  \
                [94, 98, 166, 30, 225, 300, 299, 455, 442, 337, 372, 250, 196, 429, 256, 204, 338, 427, 38, 247, 256, 94, 372, 166, 462, 455, 383, 338, 30, 225, 98, 102, 337, 282, 128, 177, 247, 168, 442, 375, 94, 166, 30, 98, 225, 299, 300, 442, 337, 455, 250, 196, 372, 429, 338, 427, 204, 496, 462, 256, 30, 299, 166, 225, 94, 442, 300, 372, 337, 196, 462, 215, 98, 496, 250, 338, 36, 40, 429, 444, 300, 30, 94, 225, 299, 166, 196, 442, 250, 204, 337, 98, 496, 462, 429, 62, 455, 372, 40, 215, 94, 300, 166, 30, 204, 225, 462, 196, 128, 455, 256, 247, 383, 337, 98, 165, 86, 102, 372, 442, 300, 94, 30, 166, 225, 196, 204, 299, 442, 462, 337, 455, 98, 250, 128, 372, 247, 165, 86, 256, 30, 299, 94, 225, 166, 442, 300, 196, 98, 337, 496, 250, 215, 372, 40, 429, 462, 36, 255, 62, 94, 30, 299, 225, 166, 300, 442, 98, 196, 250, 337, 496, 204, 429, 455, 372, 40, 462, 62, 215, 256, 372, 166, 94, 462, 383, 338, 455, 30, 225, 102, 282, 337, 98, 128, 177, 247, 168, 69, 36, 94, 30, 166, 225, 300, 299, 442, 98, 337, 196, 372, 462, 455, 250, 204, 429, 215, 496, 338, 256, 30, 94, 166, 225, 300, 299, 442, 196, 337, 462, 372, 98, 250, 204, 215, 455, 496, 429, 62, 36, 30, 299, 94, 225, 166, 442, 300, 196, 337, 98, 250, 496, 372, 215, 462, 40, 429, 36, 62, 204, 128, 383, 168, 165, 282, 86, 247, 98, 156, 256, 97, 375, 338, 4, 94, 293, 277, 468, 177, 108, 300, 94, 30, 166, 225, 196, 299, 204, 442, 337, 98, 250, 462, 455, 247, 165, 86, 128, 372, 429, 30, 94, 166, 225, 299, 300, 442, 196, 337, 98, 372, 250, 462, 496, 215, 455, 429, 40, 36, 338, 94, 256, 166, 455, 300, 30, 462, 383, 98, 225, 128, 372, 247, 102, 204, 337, 177, 282, 86, 196, 94, 300, 30, 166, 225, 98, 299, 196, 204, 442, 250, 337, 455, 429, 247, 462, 372, 128, 86, 38, 94, 30, 300, 166, 225, 299, 98, 196, 442, 250, 337, 204, 455, 429, 372, 462, 496, 62, 247, 38, 94, 30, 166, 225, 299, 98, 300, 442, 337, 196, 372, 455, 250, 429, 462, 496, 338, 204, 36, 215, 300, 94, 299, 30, 225, 250, 98, 196, 166, 204, 442, 496, 429, 255, 427, 40, 337, 62, 38, 483, 94, 30, 299, 300, 225, 166, 442, 196, 98, 250, 337, 496, 204, 429, 372, 462, 40, 455, 62, 215, 30, 300, 94, 225, 166, 299, 196, 442, 462, 337, 204, 250, 372, 215, 496, 62, 165, 98, 247, 86, 94, 98, 166, 338, 225, 30, 372, 299, 442, 455, 256, 337, 120, 36, 429, 444, 427, 462, 255, 156, 94, 166, 30, 225, 300, 462, 442, 372, 299, 337, 98, 196, 455, 256, 204, 247, 128, 383, 250, 102, 94, 166, 30, 225, 372, 98, 256, 462, 455, 337, 300, 442, 338, 299, 383, 247, 196, 128, 177, 36, 94, 300, 166, 30, 225, 462, 256, 455, 204, 196, 383, 337, 372, 128, 247, 98, 102, 442, 86, 165, 94, 30, 166, 225, 299, 442, 300, 372, 98, 337, 462, 455, 196, 338, 256, 36, 250, 215, 429, 496, 299, 30, 94, 225, 166, 442, 300, 196, 496, 250, 98, 337, 215, 40, 372, 429, 462, 36, 255, 62, 300, 94, 30, 166, 225, 196, 299, 204, 442, 337, 98, 250, 462, 455, 247, 372, 165, 128, 86, 429, 30, 94, 299, 225, 166, 300, 442, 196, 337, 250, 98, 496, 462, 372, 215, 429, 204, 40, 62, 455, 94, 166, 300, 30, 98, 225, 455, 256, 204, 128, 247, 337, 462, 372, 196, 383, 102, 86, 177, 165, 299, 30, 94, 225, 166, 442, 300, 98, 196, 250, 496, 337, 40, 429, 215, 372, 255, 62, 36, 462, 94, 30, 166, 225, 299, 98, 300, 442, 337, 250, 196, 455, 372, 429, 496, 204, 462, 338, 40, 427, 94, 300, 30, 166, 225, 196, 299, 204, 98, 442, 337, 250, 455, 462, 429, 247, 128, 372, 86, 165, 94, 30, 166, 225, 299, 442, 372, 98, 300, 337, 462, 338, 196, 455, 36, 215, 256, 250, 496, 429, 94, 166, 30, 225, 372, 98, 299, 442, 300, 337, 462, 455, 338, 256, 196, 36, 215, 429, 247, 250, 94, 300, 98, 204, 455, 128, 247, 166, 256, 86, 165, 30, 196, 225, 102, 177, 427, 375, 38, 282, 299, 30, 94, 225, 166, 442, 300, 98, 496, 250, 196, 337, 429, 255, 40, 372, 215, 36, 292, 338, 94, 30, 300, 166, 225, 98, 299, 196, 442, 455, 337, 204, 250, 372, 462, 429, 247, 256, 128, 38, 94, 30, 166, 225, 299, 442, 300, 98, 372, 337, 462, 196, 455, 338, 250, 215, 36, 256, 496, 429, 94, 30, 225, 166, 299, 300, 442, 98, 196, 337, 250, 372, 496, 462, 429, 215, 40, 455, 204, 62, 30, 94, 166, 225, 299, 300, 442, 196, 337, 98, 372, 250, 462, 496, 215, 455, 429, 40, 36, 204, 299, 30, 225, 94, 166, 442, 300, 496, 98, 337, 196, 250, 215, 372, 40, 255, 429, 36, 338, 462, 299, 30, 166, 225, 94, 442, 372, 338, 98, 496, 215, 337, 36, 40, 300, 444, 462, 120, 250, 255, 94, 166, 30, 225, 299, 442, 300, 98, 372, 337, 462, 338, 455, 196, 256, 36, 215, 250, 429, 496, 94, 300, 166, 30, 225, 455, 98, 204, 196, 256, 337, 462, 128, 247, 372, 383, 86, 102, 442, 165, 94, 30, 166, 225, 300, 299, 98, 442, 337, 196, 372, 455, 462, 250, 204, 429, 256, 247, 496, 338, 166, 94, 30, 225, 462, 372, 300, 442, 256, 337, 383, 455, 299, 338, 196, 98, 128, 247, 102, 215, 30, 94, 166, 225, 299, 300, 442, 337, 196, 372, 462, 98, 250, 215, 455, 496, 36, 338, 429, 204, 94, 166, 30, 225, 98, 299, 300, 442, 372, 337, 455, 462, 196, 338, 256, 250, 429, 247, 36, 496, 30, 299, 166, 94, 225, 442, 372, 98, 338, 337, 300, 496, 215, 36, 462, 40, 250, 196, 444, 429, 94, 30, 166, 225, 299, 300, 442, 98, 337, 372, 196, 462, 455, 250, 338, 496, 429, 215, 36, 256, 256, 94, 383, 455, 462, 166, 128, 102, 372, 247, 282, 177, 98, 30, 300, 225, 337, 204, 86, 375, 94, 30, 166, 300, 225, 462, 196, 442, 337, 299, 372, 204, 455, 256, 383, 98, 128, 247, 102, 86, 94, 166, 30, 225, 299, 442, 98, 372, 338, 337, 300, 36, 462, 215, 496, 429, 120, 455, 40, 250, 300, 30, 94, 225, 299, 166, 196, 442, 250, 204, 337, 98, 496, 462, 429, 62, 455, 372, 40, 215, 30, 94, 166, 225, 299, 300, 442, 196, 337, 98, 372, 462, 250, 496, 215, 455, 429, 36, 40, 338, 299, 30, 94, 225, 166, 442, 300, 98, 250, 496, 196, 337, 429, 40, 255, 372, 215, 36, 62, 292, 94, 98, 166, 30, 225, 300, 299, 455, 442, 337, 372, 250, 196, 429, 338, 256, 427, 204, 38, 462, 94, 30, 299, 166, 225, 442, 300, 98, 337, 196, 250, 496, 372, 429, 215, 40, 462, 455, 338, 36, 94, 166, 30, 225, 98, 299, 300, 442, 337, 372, 455, 196, 462, 338, 256, 250, 429, 204, 247, 36, 94, 166, 30, 225, 299, 442, 300, 372, 98, 337, 462, 338, 455, 196, 256, 36, 215, 250, 429, 496, 94, 30, 300, 166, 225, 299, 442, 196, 98, 337, 204, 250, 462, 455, 372, 429, 496, 247, 62, 215, 299, 30, 94, 225, 166, 442, 98, 300, 496, 250, 196, 337, 255, 40, 429, 215, 372, 36, 338, 292]

        assert NdcgScore(data, preds, top_k=20) == pytest.approx(0.7361758211688317, abs=5*1e-3)
        preds2 = exp.recommend(users = [1, 2], exclude_known=False)
        assert preds2['user'].unique().tolist() == [1,2]

        preds = exp.recommend(exclude_known=True)
        joined_preds = preds.join(
            data.set_index(['user', 'item']),
            on=['user','item'], how='inner', rsuffix="_right"
        )
        assert len(joined_preds) == 0
