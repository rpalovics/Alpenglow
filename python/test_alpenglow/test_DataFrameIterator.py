import alpenglow as prs
import alpenglow.Getter as rs
import alpenglow.experiments
import alpenglow.evaluation
import pandas as pd
import math


class TestDataFrameIterator:
    def test_dataFrameIterator(self):
        data = pd.read_csv(
            "python/test_alpenglow/test_data_4",
            sep=' ',
            header=None,
            names=['time', 'user', 'item', 'id', 'score', 'eval']
        )
        data['id']+=5
        data['eval']=0
        data.iloc[len(data)//2:]['eval']=1
        data.set_index('id')
        factorModelExperiment = alpenglow.experiments.FactorModelExperiment(
            top_k=100,
            seed=254938879,
            dimension=10,
            learning_rate=0.1,
            negative_rate=10
        )
        ranks = factorModelExperiment.run(data, verbose=True)
        assert list(ranks.index) == list(data[data['eval']==1].index)