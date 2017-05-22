import pandas as pd
import math


class NdcgScore:
    def __init__(self, rankings):
        treshold = rankings.top_k
        logs = rankings.logs
        ndcgs = []
        for l in logs:
            ndcg = 0
            if l.rank < treshold:
                ndcg = math.log(2) / math.log(l.rank + 2)
            ndcgs.append((l.time, ndcg))
        self.scores = pd.DataFrame.from_records(ndcgs, columns=('time', 'ndcg')).set_index('time')

    def time_frame(self, time_frame):
        if time_frame is None:
            return self.scores
        else:
            self.scores.reset_index(inplace=True)
            dfm = self.scores.groupby(self.scores['time'] // time_frame).agg({'time': 'first', 'ndcg': 'mean'})
            return dfm.set_index('time')

    def time_frame(self, time_frame):
      return self.time_frame(time_frame)

    def cumulative(self):
        return self.scores.mean().values[0]

    def evaluate(self):
        return self.cumulative()
