import pandas as pd
import math


class DcgScore:
    def __init__(self, rankings):
        treshold = rankings.top_k
        logs = rankings.logs
        dcgs = []
        for l in logs:
            dcg = 0
            if l.rank < treshold:
                dcg = math.log(2) / math.log(l.rank + 2)
            dcgs.append((l.time, dcg))
        self.scores = pd.DataFrame.from_records(dcgs, columns=('time', 'dcg')).set_index('time')

    def time_frame(self, time_frame):
        if time_frame is None:
            return self.scores
        else:
            self.scores.reset_index(inplace=True)
            dfm = self.scores.groupby(self.scores['time'] // time_frame).agg({'time': 'first', 'dcg': 'mean'})
            return dfm.set_index('time')

    def cumulative(self):
        return self.scores.mean().values[0]

    def evaluate(self):
        return self.cumulative()
