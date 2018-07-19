import alpenglow as prs
import alpenglow.Getter as rs
import alpenglow.evaluation
import pandas as pd
import math
import unittest


class TestRrScore(unittest.TestCase):
    def test_rrScore(self):
        ranks = [102, 102, 102, 102, 3, 4, 102, 9, 102, 102, 102, 9, 102, 102, 102, 6, 102, 102, 102, 102, 102, 102, 102, 1, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 12, 102, 1, 102, 102, 102, 1, 102, 102, 102, 102, 65, 1, 12, 12, 10, 102, 102, 102, 102, 102]
        facRankings = pd.DataFrame.from_records(
            [
                (i, i, 0, r if r <= 10 else None)
                for i, r in enumerate(ranks)
            ],
            columns=["id", "time", "prediction", "rank"]
        ).set_index("id")
        facRankings.top_k = 10
        rr = alpenglow.evaluation.RrScore(facRankings).mean()
        rrs = [1.0 / r if r <= 10 else 0 for r in ranks]
        self.assertAlmostEqual(rr, sum(rrs) / len(rrs))
