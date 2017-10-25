import alpenglow.cpp as rs
import numpy as np
import sip


class DataframeData(rs.DataframeData):
    """Python wrapper around :py:class:`alpenglow.cpp.DataframeData`.
    """
    def __init__(self, df, columns={}):
        super(rs.DataframeData, self).__init__()
        self.columns = columns
        self.position = 0
        self.df = df

        users = np.array(self._get_def_valarray('user'), dtype=np.intc)
        items = np.array(self._get_def_valarray('item'), dtype=np.intc)
        ids = np.array(df.index, dtype=np.intc)
        scores = np.array(self._get_def_valarray('score', 'ones'), dtype=np.float64)
        times = np.array(self._get_def_valarray('time', 'arange'), dtype=np.float64)
        evals = np.array(self._get_def_valarray('eval', 'ones'), dtype=np.intc)
        categories = np.array(self._get_def_valarray('category', 'ones'), dtype=np.intc)

        self.add_recdats(users,items,ids,scores,times,evals,categories)

        super(rs.DataframeData, self).initialize()

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