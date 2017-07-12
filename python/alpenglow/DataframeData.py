import alpenglow.cpp as rs
import numpy as np
import sip


class DataframeData(rs.DataframeData):
    def __init__(self, df, columns={}):
        super(rs.DataframeData, self).__init__()
        self.columns = columns
        self.position = 0
        self.df = df

        users = self._get_def_valarray('user')
        items = self._get_def_valarray('item')
        ids = list(df.index)
        scores = self._get_def_valarray('score', 'ones')
        times = self._get_def_valarray('time', 'arange')
        evals = self._get_def_valarray('eval', 'ones')
        categories = self._get_def_valarray('category', 'ones')

        indexes = np.arange(len(self.df))
        recdats = []
        for (_i, _user, _item, _id, _score, _time, _eval, _category) in zip(indexes, users, items, ids, scores, times, evals, categories):
            rd = rs.RecDat()
            rd.user = _user
            rd.item = _item
            rd.id = _id
            rd.score = _score
            rd.time = _time
            rd.eval = _eval
            rd.category = _category
            recdats.append(rd)
            sip.transferto(rd, None)
        self.add_recdats(recdats)
        print("initing dataframedata from py", super(rs.DataframeData, self).initialize())

    def _get_def_valarray(self, name, deftype=None):
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