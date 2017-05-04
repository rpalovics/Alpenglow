import alpenglow.cpp as rs
import numpy as np


class DataframeIterator(rs.DataframeIterator):
    def __init__(self, df, columns={}):
        super(rs.DataframeIterator, self).__init__()
        self.columns = columns
        self.position = 0
        self.df = df

    def _getDefValarray(self, name, deftype=None):
        if(name in self.columns):
            return self.df[self.columns[name]].values

        if(name in self.df.columns and self.columns == {}):
            return self.df[name].values

        else:
            if(deftype == 'arange'):
                return np.arange(len(self.df))
            elif(deftype == 'ones'):
                return np.ones(len(self.df))
            else:
                raise ValueError('Nonexistent column ' + name)

    def init(self):
        users = self._getDefValarray('user')
        items = self._getDefValarray('item')
        ids = self._getDefValarray('id', 'arange')
        scores = self._getDefValarray('score', 'ones')
        times = self._getDefValarray('time', 'arange')
        evals = self._getDefValarray('eval', 'ones')
        categories = self._getDefValarray('category', 'ones')

        indexes = np.arange(len(self.df))
        for (i_, user_, item_, id_, score_, time_, eval_, category_) in zip(indexes, users, items, ids, scores, times, evals, categories):
            rd = rs.RecDat()
            rd.user = user_
            rd.item = item_
            rd.id = id_
            rd.score = score_
            rd.time = time_
            rd.eval = eval_
            rd.category = category_
            self.add_recdat(rd)
