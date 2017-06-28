import alpenglow as prs
import alpenglow.experiments
import alpenglow.evaluation
import pandas as pd
import math
import unittest


class TestThreadedParameterSearch(unittest.TestCase):
    def test_runMultiple(self):
        data = pd.read_csv(
            "python/test_alpenglow/test_data_4",
            sep=' ',
            header=None,
            names=['time', 'user', 'item', 'id', 'score', 'eval']
        )
        model = alpenglow.experiments.PopularityModelExperiment(
            top_k=100,
            seed=254938879
        )
        c = prs.ParameterSearch(model, alpenglow.evaluation.DcgScore)
        c.set_parameterValues('top_k', [100, 50])
        c.set_parameterValues('seed', [254938879, 0])

        d = prs.ThreadedParameterSearch(model, alpenglow.evaluation.DcgScore)
        d.set_parameterValues('top_k', [100, 50])
        d.set_parameterValues('seed', [254938879, 0])

        r1 = c.run(data)
        r2 = d.run(data)
        assert r1.equals(r2)
