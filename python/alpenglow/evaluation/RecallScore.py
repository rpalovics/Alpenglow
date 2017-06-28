import pandas as pd
import numpy as np
import math


def Recall(rank, top_k):
    if(np.isnan(rank)):
        return 0
    else:
        return 1 / top_k


def RecallScore(rankings, top_k=None):
    if top_k is None:
        try:
            top_k = rankings.top_k
        except Exception as e:
            raise RuntimeError("Missing top_k value!")
    return rankings['rank'].map(lambda r: Recall(r, top_k))
