import pandas as pd
import numpy as np
import math

def Precision(rank):
    if(np.isnan(rank)):
        return 0
    else:
        return 1


def PrecisionScore(rankings):
    return rankings['rank'].map(Precision)
