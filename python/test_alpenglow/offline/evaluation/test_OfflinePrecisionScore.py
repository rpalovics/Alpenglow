from alpenglow.offline.evaluation import PrecisionScore
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
        assert PrecisionScore(test_data, ranks, 5) == np.mean([1, 0.2, 0])