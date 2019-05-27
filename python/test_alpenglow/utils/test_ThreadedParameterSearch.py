import alpenglow as prs
import alpenglow.experiments
import alpenglow.evaluation
import pandas as pd
import math
import unittest
from alpenglow.utils import ParameterSearch, ThreadedParameterSearch


class TestThreadedParameterSearch(unittest.TestCase):
    def test_runMultiple(self):
        data = pd.read_csv(
            "python/test_alpenglow/test_data_4",
            sep=' ',
            header=None,
            names=['time', 'user', 'item', 'id', 'score', 'eval']
        )
        model = alpenglow.experiments.PopularityExperiment(
            top_k=100,
            seed=254938879
        )
        c = ParameterSearch(model, alpenglow.evaluation.DcgScore)
        c.set_parameter_values('top_k', [100, 50])
        c.set_parameter_values('seed', [254938879, 123456])

        d = ThreadedParameterSearch(model, alpenglow.evaluation.DcgScore)
        d.set_parameter_values('top_k', [100, 50])
        d.set_parameter_values('seed', [254938879, 123456])

        r1 = c.run(data)
        r2 = d.run(data)
        assert r1.equals(r2)
