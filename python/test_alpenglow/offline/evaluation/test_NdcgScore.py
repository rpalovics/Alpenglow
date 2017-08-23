from alpenglow.offline.evaluation import NdcgScore
import numpy as np
import math
import unittest
import pandas

class TestNdcgScore(unittest.TestCase):
    def test_implicit(self):
        ranks = pandas.DataFrame({
            'user':[1,1,1,1,1,2,2,2,2,2,3,3,3,3],
            'item':[1,2,3,4,5,1,2,3,4,5,1,2,3,4],
            'rank':[1,2,3,4,5,1,2,3,4,5,1,2,3,4],
        })
        test_data = pandas.DataFrame({
            'user': [1,1,1,1,1,1,1,2,2,3],
            'item': [1,2,3,4,5,6,7,2,10,10],
            'score':[1,1,1,1,1,1,1,1,1,1]
        })

        u2ndcg = (1/math.log(2+1,2)) / (1/math.log(2,2)+1/math.log(3,2))
        assert NdcgScore(test_data, ranks, 5) == np.mean([1, u2ndcg, 0])

    def test_explicit(self):
        ranks = pandas.DataFrame({
            'user':[1,1,1,1,1,2,2,2,2,2,3,3,3,3],
            'item':[1,2,3,4,5,1,2,3,4,5,1,2,3,4],
            'rank':[1,2,3,4,5,1,2,3,4,5,1,2,3,4],
        })
        test_data = pandas.DataFrame({
            'user': [1,1,1,1,1,1,1,2,2,3],
            'item': [1,2,3,4,5,6,7,2,10,10],
            'score':[8,6,4,3,2,1,1,10,5,1]
        })

        u2ndcg = (10/math.log(3,2)) / (10/math.log(2,2)+5/math.log(3,2))
        self.assertAlmostEqual(np.mean([1, u2ndcg, 0]),NdcgScore(test_data, ranks, 5))