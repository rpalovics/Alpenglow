import pandas as pd
import numpy as np
import math


def Dcg(rank):
    if(np.isnan(rank)):
        return 0
    else:
        return math.log(2) / math.log(rank + 1)


def DcgScore(rankings):
    return rankings['rank'].map(Dcg)
