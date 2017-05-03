import alpenglow as prs
import alpenglow.experiments
import pandas as pd
import math
import unittest

class TestThreadedParameterSearch(unittest.TestCase):
    def test_runMultiple(self):
        data = pd.read_csv(
            "/mnt/idms/recSys/examples/sample_data/online/online_lastfm_10000", 
            sep=' ', 
            header=None, 
            names=['time','user','item','score','eval']
        )
        model = alpenglow.experiments.PopularityModelExperiment(
            topK = 100,
            seed = 254938879
        )
        c = prs.ParameterSearch(model, prs.NdcgScore)
        c.setParameterValues('topK', [100,50])
        c.setParameterValues('seed', [254938879, 0])

        d = prs.ThreadedParameterSearch(model, prs.NdcgScore)
        d.setParameterValues('topK', [100,50])
        d.setParameterValues('seed', [254938879, 0])
        
        r1 = c.run(data)
        r2 = d.run(data)
        assert r1.equals(r2)