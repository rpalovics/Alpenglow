import pandas as pd
import numpy as np
import math


def Rr(rank):
    if(np.isnan(rank)):
        return 0
    else:
        return 1.0/rank


def RrScore(rankings):
    """
       Reciprocial rank, see https://en.wikipedia.org/wiki/Mean_reciprocal_rank .
    """
    return rankings['rank'].map(Rr)
