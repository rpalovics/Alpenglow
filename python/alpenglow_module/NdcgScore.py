import pandas as pd
import math


class NdcgScore:
    def __init__(self, rankings):
        treshold = rankings.topK
        logs = rankings.logs
        ndcgs = []
        for l in logs:
            ndcg = 0
            if l.rank < treshold:
                ndcg = math.log(2) / math.log(l.rank + 2)
            ndcgs.append((l.time, ndcg))
        self.scores = pd.DataFrame.from_records(ndcgs, columns=('time', 'ndcg')).set_index('time')

    def timeFrame(self, timeFrame):
        if timeFrame is None:
            return self.scores
        else:
            self.scores.reset_index(inplace=True)
            dfm = self.scores.groupby(self.scores['time'] // timeFrame).agg({'time': 'first', 'ndcg': 'mean'})
            return dfm.set_index('time')

    def cumulative(self):
        return self.scores.mean().values[0]

    def evaluate(self):
        return self.cumulative()
